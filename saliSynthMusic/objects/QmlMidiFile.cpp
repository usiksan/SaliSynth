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
#include "synthConfig.h"
#include "QmlMidiFile.h"
#include "SvQml/SvDir.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFileInfo>
#include <QDir>
#include <QDebug>


static const char (*trackColors[12]) = { "#267F00", "#273EE8", "#8522E4", "#F01D83",
                                         "#11F371", "#4A8BE7", "#CB24ED", "#F41E46",
                                         "#51EACA", "#4FCDED", "#F020D8", "#FD4920"
                                       };


QmlMidiFile::QmlMidiFile(QObject *parent) :
  QObject(parent),
  mConfigDirty(false),
  mTickCount(-1),
  mTickStep(16),
  mPause(false)
  {
  //Fill fields of track model
  mQmlTrackModel.setFields( { TRACK_ID, TRACK_NAME, TRACK_INDEX, TRACK_ON, TRACK_REMARK, TRACK_VOLUME,
                              TRACK_VISIBLE, TRACK_COLOR } );
  connect( &mQmlTrackModel, &SvQmlJsonModel::afterModelChanged, this, [this] () { mConfigDirty = true; } );

  //Midi event from all tracks we send to midi event signal of QmlMidiFile
  for( int i = 0; i < 16; i++ )
    connect( &(mQmlTrack[i]), &QmlMidiTrack::midiEvent, this, &QmlMidiFile::midiEvent );

  //Timer for periodic tick generation
  connect( &mTimer, &QTimer::timeout, this, &QmlMidiFile::tick );
  mTimer.start( TICK_PERIOD_MS );
  }





bool QmlMidiFile::read(QString fname)
  {
  mOriginalFile = fname;

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

  mMarkerList.clear();

  //Repeated tracks
  int trackIndex = 0;
  while( !reader.isEnd() ) {
    IffReader track = reader.getChunk();
    qDebug() << "midi reader chunk" << track.name();
    if( track.compareChunkName("MTrk") ) {
      readMtrk( trackIndex, track );
      trackIndex++;
      }
    else readExtension( track );
    }

  //Clear all tracks
  quint8 channel = 4;
  mQmlTrackModel.clear();
  for( int i = 0; i < 16; i++ ) {
//    qDebug() << "endTrack" << i;
    mQmlTrack[i].endReadTrack();
    if( mQmlTrack[i].isActive() ) {

      //qDebug() << "Track" << i << " mapped to" << channel;
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
      mQmlTrackModel.setInt( r, TRACK_VISIBLE, 1 );
      mQmlTrackModel.setString( r, TRACK_COLOR, QString::fromLatin1( trackColors[r] ) );
      }
    if( channel >= 16 ) break;
    }

  //Read config file if present
  configRead( fname );

  //Execute post read
  postRead();

  //At and we collect all files from fname directory
  QFileInfo info(fname);
  QDir dir( SvDir(info.absolutePath()).slashedPath() );
  QFileInfoList list = dir.entryInfoList( { QString("*.%1").arg(info.suffix()) }, QDir::Files );
  mFileList.clear();
  for( auto const &inf : list )
    if( inf.isFile() )
      mFileList.append( inf.filePath() );
  //qDebug() << "file list" << mFileList;
  return true;
  }






void QmlMidiFile::readPrev()
  {
  //Stop play
  stop();

  //Find original file name in file list
  int i = mFileList.indexOf( mOriginalFile );
  //If found we read previous file
  if( i > 0 ) read( mFileList.at(i - 1) );
  }




void QmlMidiFile::readNext()
  {
  //Stop play
  stop();

  //Find original file name in file list
  int i = mFileList.indexOf( mOriginalFile );
  //If found we read next file
  if( i >= 0 && i < (mFileList.count() - 1) ) read( mFileList.at(i + 1) );
  }





void QmlMidiFile::configRead(QString fname)
  {
  //Construct cfg file name from midi file name
  QFileInfo info(fname);
  mConfigFile = info.absolutePath();
  if( !mConfigFile.endsWith(QChar('/')) )
    mConfigFile.append( QChar('/') );
  mConfigFile += info.completeBaseName() + QString(".cfg");

  mMidiName = info.completeBaseName();
  emit midiNameChanged();

  //qDebug() << "midi config" << mConfigFile;

  mConfigDirty = true;
  mPause = false;
  mTickCount = -1;

  //Calculate mTickStep on default tempo
  auto usPerTick = mTempo / mDivision;
  //Our tick = 20000us
  mTickStep = TICK_PERIOD_US * 16 / usPerTick;

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
  emit tempoChanged();
  }





void QmlMidiFile::setTickCount(int tc)
  {
  mTickCount = tc << 4;
  for( int i = 0; i < 16; i++ )
    mQmlTrack[i].seek( tc );
  emit tickCountChanged();
  }




int QmlMidiFile::tempo() const
  {
  //mTickStep - tick count of TICK_PERIOD_US / 16
  //usPerTick = TICK_PERIOD_US / (mTickStep / 16) = TICK_PERIOD_US * 16 / mTickStep
  //mDivision - tick per quarter
  //usPerQuarter = mDivision * usPerTick = mDivision * TICK_PERIOD_US * 16 / mTickStep
  quint64 usPerQuarter = mDivision;
  usPerQuarter *= TICK_PERIOD_US * 16;
  usPerQuarter /= mTickStep;
  return 60'000'000 / usPerQuarter;
  }





void QmlMidiFile::setTempo(int aTempo)
  {
  quint64 usPerQuarter = 60'000'000 / aTempo;
  quint64 temp = mDivision;
  temp *= TICK_PERIOD_US * 16;
  mTickStep = temp / usPerQuarter;
  emit tempoChanged();

  mMetronomePeriod = usPerQuarter / TICK_PERIOD_MS;
  mMetronomeCount = 0;
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
  if( mTickCount >= 0 && !mPause ) {
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

  //Metronome
  mMetronomeCount += 1000;
  if( mMetronomeCount >= mMetronomePeriod ) {
    mMetronomeCount -= mMetronomePeriod;
    //qDebug() << "metronome";
    emit metronome();
    }
  }





void QmlMidiFile::play()
  {
  if( mPause )
    //Was a pause
    mPause = false;
  else {
    //Mid was stopped
    //Setup all voices
    for( int i = 0; i < mQmlTrackModel.count(); i++ ) {
      int trackIndex = mQmlTrackModel.asInt( i, TRACK_INDEX );
      emit voiceSetup( mQmlTrack[trackIndex].channel(), mQmlTrackModel.asInt( i, TRACK_ID ) );
      }

    setTickCount(0);
    }
  }




//!
//! \brief pause Pause playing
//!
void QmlMidiFile::pause()
  {
  //Stop all playing notes
  stopAllNotes();
  //... and set pause mode
  mPause = true;
  }




//!
//! \brief stop Stop play
//!
void QmlMidiFile::stop()
  {
  //Stop all playing notes
  stopAllNotes();

  //And reset tick count, which stops playing
  mTickCount = -1;
  mPause = false;
  }





bool QmlMidiFile::readMthd(IffReader &reader)
  {
  qDebug() << reader.name();
  mFormat      = reader.getUint16be();
  mTrackNumber = reader.getUint16be();
  mDivision    = reader.getUint16be();

  mTempo = mPortion = mFraction = mMetronom = m32PerQuarter = 0;

  qDebug() << mFormat << mTrackNumber << "ticks per quarter" << mDivision;
  return true;
  }







void QmlMidiFile::readMtrk(int trackIndex, IffReader &reader)
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
  bool    drumChannel = false;
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
        QmlMidiMarker marker;
        marker.mMarker = QString::fromLatin1( ar );
        marker.mTime   = time;
        //Append marker to list
        mMarkerList.append( marker );
        qDebug() << "marker" << marker.mMarker << marker.mTime;
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
        qDebug() << "tempo" << mTempo;
        }

      else if( metaEvent == 0x54 ) {
        //SMPTE offset

        }

      else if( metaEvent == 0x58 ) {
        //Time signature
        mPortion = ar[0];
        mFraction = ar[1];
        mMetronom = ar[2];
        m32PerQuarter = ar[3];
        qDebug() << "portion" << mPortion;
        qDebug() << "fraction" << mFraction;
        qDebug() << "metronom" << mMetronom;
        qDebug() << "count 1/32" << m32PerQuarter;
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
      if( mFormat == 0 ) trackIndex = channelIndex;
      if( channelIndex == 9 && !drumChannel ) {
        //Traditional setup channel 9 (if count from 1 then 10) to standard drum kit
        SfVoiceId id9;
        id9.mBankMsb = 125;
        id9.mBankLsb = 0;
        id9.mProgramm = 0;
        mQmlTrack[trackIndex].setVoiceId( id9.mVoiceId );
        drumChannel = true;
        }
      mQmlTrack[trackIndex].addMidiEvent( time, groupStatusByte, data0, data1 );
      }
    }

  mQmlTrack[trackIndex].setTrackName( trackName );
  mQmlTrack[trackIndex].setInstrumentName( instrumentName );

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




void QmlMidiFile::postRead()
  {
  //Do nothing
  }




//!
//! \brief stopAllNotes Stop all playing notes
//!
void QmlMidiFile::stopAllNotes()
  {
  //End all continuously notes
  for( int i = 0; i < mQmlTrackModel.count(); i++ )
    mQmlTrack[mQmlTrackModel.asInt( i, TRACK_INDEX )].stop();
  }

