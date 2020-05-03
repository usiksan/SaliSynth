/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   MIDI file

   login: usiksan
   Password: auz9P7sLG8Cj44q
*/
#include "QmlMidiFile.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFileInfo>
#include <QDebug>

#define TRACK_ID     QStringLiteral("trackId")
#define TRACK_NAME   QStringLiteral("trackName")
#define TRACK_INDEX  QStringLiteral("trackIndex")
#define TRACK_ON     QStringLiteral("trackOn")
#define TRACK_REMARK QStringLiteral("trackRemark")
#define TRACK_VOLUME QStringLiteral("trackVolume")

QmlMidiFile::QmlMidiFile(QObject *parent) :
  QObject(parent),
  mConfigDirty(false),
  mTickCount(-1),
  mTickStep(16)
  {
  mQmlTrackModel.setFields( { TRACK_ID, TRACK_NAME, TRACK_INDEX, TRACK_ON, TRACK_REMARK, TRACK_VOLUME } );
  connect( &mQmlTrackModel, &SvQmlJsonModel::afterModelChanged, this, [this] () { mConfigDirty = true; } );

  for( int i = 0; i < 16; i++ )
    connect( mQmlTrack + i, &QmlMidiTrack::midiEvent, this, &QmlMidiFile::midiEvent );

  connect( &mTimer, &QTimer::timeout, this, &QmlMidiFile::tick );
  mTimer.start( 20 );
  }





bool QmlMidiFile::read(QString fname)
  {
  IffReader reader(fname, false);
  if( !reader.isValid() )
    return false;

  //Header
  if( !reader.readChunk( "MThd", [this] (IffReader &reader) -> bool { return readMthd(reader); }) )
    return false;

  //Clear all tracks
  for( int i = 0; i < 16; i++ )
    mQmlTrack[i].beginReadTrack();

  setFileLenght(0);

  //Repeated tracks
  int trackIndex = 0;
  while( !reader.isEnd() ) {
    IffReader track = reader.getChunk();
    qDebug() << "midi reader chunk" << track.name();
    if( track.compareChunkName("MTrk") ) {
      readMtrk( track );
      trackIndex++;
      }
    else readExtension( track );
    }

  //Clear all tracks
  quint8 channel = 4;
  mQmlTrackModel.clear();
  for( int i = 0; i < 16; i++ ) {
    mQmlTrack[i].endReadTrack();
    if( mQmlTrack[i].isActive() ) {

      //Setup channel for track
      mQmlTrack[i].setChannel( channel++ );

      //Setup visual representation for track info
      int r = mQmlTrackModel.count();
      mQmlTrackModel.addRecord();
      mQmlTrackModel.setInt( r, TRACK_ID, mQmlTrack[i].voiceId() );
      mQmlTrackModel.setString( r, TRACK_NAME, mQmlTrack[i].trackName() );
      mQmlTrackModel.setInt( r, TRACK_INDEX, i );
      mQmlTrackModel.setInt( r, TRACK_ON, 1 );
      mQmlTrackModel.setString( r, TRACK_REMARK, mQmlTrack[i].mRemark );
      mQmlTrackModel.setInt( r, TRACK_VOLUME, 127 );
      }
    if( channel >= 16 ) break;
    }

  //Read config file if present
  configRead( fname );

  return true;
  }





void QmlMidiFile::configRead(QString fname)
  {
  //Construct cfg file name from midi file name
  QFileInfo info(fname);
  mConfigFile = info.absolutePath();
  if( !mConfigFile.endsWith(QChar('/')) )
    mConfigFile.append( QChar('/') );
  mConfigFile += info.completeBaseName() + QString(".cfg");

  //qDebug() << "midi config" << mConfigFile;

  mConfigDirty = true;

  //Test if config file exist
  if( QFile::exists(mConfigFile) ) {
    QFile file(mConfigFile);
    if( file.open(QIODevice::ReadOnly) ) {
      //qDebug() << "midi config read" << mConfigFile;
      QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();
      mTickStep = obj.value( QStringLiteral("tickStep") ).toInt();
      mQmlTrackModel.setArray( obj.value( QStringLiteral("tracks") ).toArray() );
      mConfigDirty = false;
      }
    }
  }





void QmlMidiFile::setTickCount(int tc)
  {
  mTickCount = tc << 4;
  for( int i = 0; i < 16; i++ )
    mQmlTrack[i].seek( tc );
  emit tickCountChanged();
  }




void QmlMidiFile::setTickStep(int stp)
  {
  mTickStep = stp;
  emit tickStepChanged();
  }




void QmlMidiFile::configWrite()
  {
  if( !mConfigFile.isEmpty() ) {
    QFile file( mConfigFile );
    if( file.open(QIODevice::WriteOnly) ) {
      qDebug() << "midi config write" << mConfigFile;
      QJsonObject obj;
      obj.insert( QStringLiteral("tickStep"), mTickStep );
      obj.insert( QStringLiteral("tracks"), mQmlTrackModel.array() );
      file.write( QJsonDocument( obj ).toJson() );
      mConfigDirty = false;
      }
    }
  }





//!
//! \brief QmlMidiFile::tick With this function we generates midi sequence to play midi file
//!
void QmlMidiFile::tick()
  {
  if( mTickCount >= 0 ) {
    //We need send event beatween startTime and lastTime
    // startTime is mTickCount, and lastTime - is nextTime
    //At first we calculate nextTime
    int nextTime = mTickCount + mTickStep;

    //Anything need to be done when signed parts of time are different
    // signed parts not includes low 4 bits
    if( (nextTime & 0xfffffff0) != (mTickCount & 0xfffffff0) ) {

      //For each of tracks we execute tick with current times and track params
      for( int i = 0; i < mQmlTrackModel.count(); i++ ) {
        int trackIndex = mQmlTrackModel.asInt( i, TRACK_INDEX );
        bool soundOn = mQmlTrackModel.asInt( i, TRACK_ON );
        int volume = mQmlTrackModel.asInt( i, TRACK_VOLUME );
        mQmlTrack[trackIndex].tick( mTickCount >> 4, nextTime >> 4, soundOn, volume );
        }

      //Update current tick count
      mTickCount = nextTime;
      //Signal that tick count changed
      //On this signal visual elements display actual playing position
      emit tickCountChanged();
      }
    else
      //Update current tick count
      mTickCount = nextTime;
    }
  }





void QmlMidiFile::play()
  {
  //Setup all voices
  for( int i = 0; i < mQmlTrackModel.count(); i++ ) {
    int trackIndex = mQmlTrackModel.asInt( i, TRACK_INDEX );
    emit voiceSetup( mQmlTrack[trackIndex].channel(), mQmlTrackModel.asInt( i, TRACK_ID ) );
    }

  setTickCount(0);
  }



void QmlMidiFile::stop()
  {
  //End all continuously notes
  for( int i = 0; i < mQmlTrackModel.count(); i++ )
    mQmlTrack[mQmlTrackModel.asInt( i, TRACK_INDEX )].stop();

  mTickCount = -1;
  }





bool QmlMidiFile::readMthd(IffReader &reader)
  {
  qDebug() << reader.name();
  mFormat      = reader.getUint16be();
  mTrackNumber = reader.getUint16be();
  mDivision    = reader.getUint16be();

  qDebug() << mFormat << mTrackNumber << mDivision;
  return true;
  }







void QmlMidiFile::readMtrk(IffReader &reader)
  {
  quint32 time = 0;
  QString trackName;
  quint16 sequenceId = 0;
  QString copyright;
  QString instrumentName;
  QString lyric;
  QString marker;
  quint8  groupStatusByte = 0;
  int     channelIndex = 0;
  while( !reader.isEnd() ) {
    //Read midi event

    quint32 timeOffset = variableLenValue(reader);
    quint8 statusByte = reader.getUint8();
    time += timeOffset;
    //qDebug() << timeOffset << statusByte;
    if( statusByte == 0xff ) {
      //Meta-event
      quint8 metaEvent = reader.getUint8();
      quint32 len = variableLenValue(reader);
      QByteArray ar( static_cast<int>(len), 0 );
      for( quint32 i = 0; i < len; i++ )
        ar[i] = reader.getInt8();
      qDebug() << "meta" << metaEvent << len << ar;

      if( metaEvent == 0 ) {
        //sequence id
        sequenceId = static_cast<quint16>(
                       (static_cast<unsigned>(ar[0] & 0xff) << 8) |
                       static_cast<unsigned>(ar[1] & 0xff) );
        }

      else if( metaEvent == 1 ) {
        //text
        QString str = QString::fromLocal8Bit( ar );
        qDebug() << "text" << str;
        }

      else if( metaEvent == 2 ) {
        //Copyright
        copyright = QString::fromLatin1( ar );
        }

      else if( metaEvent == 3 ) {
        //Track name
        trackName = QString::fromLocal8Bit( ar );
        }

      else if( metaEvent == 4 ) {
        //Instrument name
        instrumentName = QString::fromLatin1( ar );
        qDebug() << "midi instrument name" << channelIndex << trackName;
        }

      else if( metaEvent == 5 ) {
        //Lyric
        lyric = QString::fromUtf8( ar );
        qDebug() << "lyric" << lyric;
        }

      else if( metaEvent == 6 ) {
        //Marker
        marker = QString::fromLatin1( ar );
        if( marker.length() > 3 ) {
          if( marker.at(0).isDigit() ) {
            if( marker.at(1) == QChar(' ') ) {
              int track = marker.mid( 0, 1 ).toInt() - 1;
              if( track >= 0 )
                mQmlTrack[track].mRemark = marker.mid( 2 );
              }
            else if( marker.at(1).isDigit() && marker.at(2) == QChar(' ') ) {
              int track = marker.mid( 0, 2 ).toInt() - 1;
              if( track >= 0 )
                mQmlTrack[track].mRemark = marker.mid( 3 );
              }
            }
          }
        qDebug() << "marker" << marker;
        }

      else if( metaEvent == 7 ) {
        //Cue point

        }

      else if( metaEvent == 8 ) {
        //Programm name

        }

      else if( metaEvent == 9 ) {
        //Device name

        }

      else if( metaEvent == 0x20 ) {
        //MIDI channel prefix
        channelIndex = static_cast<int>(ar[0] & 0xff);
        }

      else if( metaEvent == 0x2f ) {
        //End of track
        break;
        }

      else if( metaEvent == 0x51 ) {
        //Set tempo
        mTempo = static_cast<quint32>(
                   (static_cast<unsigned>(ar[0] & 0xff) << 16) |
                   (static_cast<unsigned>(ar[1] & 0xff) << 8) |
                   static_cast<unsigned>(ar[2] & 0xff) );
        }

      else if( metaEvent == 0x54 ) {
        //SMPTE offset

        }

      else if( metaEvent == 0x58 ) {
        //Time signature

        }

      else if( metaEvent == 0x59 ) {
        //Key signature

        }

      }
    else if( statusByte == 0xf0 ) {
      //Sys-ex
      quint32 len = variableLenValue(reader);
      QByteArray ar( static_cast<int>(len), 0 );
      for( quint32 i = 0; i < len; i++ )
        ar[i] = reader.getInt8();
      qDebug() << "sys-ex" << len << ar;
      }
    else if( (statusByte & 0xf0) == 0xf0 ) {
      //System events
      }
    else {
      //Midi-event
      quint8 data0, data1;
      if( statusByte & 0x80 ) {
        //Real status byte
        groupStatusByte = statusByte;

        if( (statusByte & 0xf0) == 0xc0 || (statusByte & 0xf0) == 0xd0 ) {
          //Programm change or Channel pressure
          data0 = reader.getUint8();
          data1 = 0;
          //qDebug("midi-1 %x %d", statusByte, event.mData0 );
          }
        else {
          data0 = reader.getUint8();
          data1 = reader.getUint8();
          //qDebug("midi-2 %x %d %d", statusByte, event.mData0, event.mData1 );
          }
        }
      else {
        //Group data
        if( (groupStatusByte & 0xf0) == 0xc0 || (groupStatusByte & 0xf0) == 0xd0 ) {
          //Programm change or Channel pressure
          data0 = statusByte;
          data1 = 0;
          //qDebug("group midi-1 %x %d", event.mType, event.mData0 );
          }
        else {
          data0 = statusByte;
          data1 = reader.getUint8();
          //qDebug("midi-2 %x %d %d", event.mType, event.mData0, event.mData1 );
          }
        }

      channelIndex = groupStatusByte & 0xf;
      mQmlTrack[channelIndex].addMidiEvent( time, groupStatusByte, data0, data1 );
      }
    }

  mQmlTrack[channelIndex].setTrackName( trackName );
  mQmlTrack[channelIndex].setInstrumentName( instrumentName );

  //Update file time lenght
  if( time > mFileLenght )
    setFileLenght(time);
  }




void QmlMidiFile::readExtension(IffReader &reader)
  {
  Q_UNUSED(reader)
  }










quint32 QmlMidiFile::variableLenValue(IffReader &reader)
  {
  quint32 value = 0;
  quint8 ch;
  do {
    ch = reader.getUint8();
    value <<= 7;
    value |= ch & 0x7f;
    }
  while( ch & 0x80 );

  return value;
  }

