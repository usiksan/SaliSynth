/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   MIDI file
*/
#include "QmlMidiFile.h"

#include <QDebug>

QmlMidiFile::QmlMidiFile(QObject *parent) :
  QObject(parent),
  mTickCount(-1)
  {
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

  //Repeated tracks
  int trackIndex = 0;
  while( !reader.isEnd() ) {
    IffReader track = reader.getChunk();
    qDebug() << track.name();
    if( track.compareChunkName("MTrk") ) {
      readMtrk( track );
      trackIndex++;
      }
    }
  return true;
  }



void QmlMidiFile::tick()
  {
  if( mTickCount >= 0 ) {
    int nextTime = mTickCount + 1;
    for( auto &track : mTracks ) {
      if( track.mEventIndex < track.mEvents.count() ) {
        MidiEvent ev = track.mEvents.at( track.mEventIndex );
        while( mTickCount <= ev.mTime && ev.mTime < nextTime ) {
          //Event at this moment
          emit midiEvent( ev.mType, ev.mData0, ev.mData1 );
          track.mEventIndex++;
          if( track.mEventIndex >= track.mEvents.count() )
            break;
          ev = track.mEvents.at( track.mEventIndex );
          }
        }
      }
    mTickCount = nextTime;
    }
  }



void QmlMidiFile::seek(quint32 time)
  {
  mTickCount = time;
  for( auto &track : mTracks )
    track.mEventIndex = 0;
  }








bool QmlMidiFile::readMthd(IffReader &reader)
  {
  qDebug() << reader.name();
  mFormat      = reader.getUint16be();
  mTrackNumber = reader.getUint16be();
  mDivision    = reader.getUint16be();

  //Build track vector
  mTracks.clear();
  //Single track in midi with some channels
  mTracks.resize(16);

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
    qDebug() << timeOffset << statusByte;
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
        }

      else if( metaEvent == 2 ) {
        //Copyright
        copyright = QString::fromLatin1( ar );
        }

      else if( metaEvent == 3 )
        //Track name
        trackName = QString::fromUtf8( ar );

      else if( metaEvent == 4 ) {
        //Instrument name
        instrumentName = QString::fromLatin1( ar );
        }

      else if( metaEvent == 5 ) {
        //Lyric
        lyric = QString::fromUtf8( ar );
        }

      else if( metaEvent == 6 ) {
        //Marker
        marker = QString::fromLatin1( ar );
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
      MidiEvent event;
      event.mTime = time;
      if( statusByte & 0x80 ) {
        //Real status byte
        groupStatusByte = statusByte;
        event.mType = statusByte;

        if( (statusByte & 0xf0) == 0xc0 || (statusByte & 0xf0) == 0xd0 ) {
          //Programm change or Channel pressure
          event.mData0 = reader.getUint8();
          event.mData1 = 0;
          qDebug("midi-1 %x %d", statusByte, event.mData0 );
          }
        else {
          event.mData0 = reader.getUint8();
          event.mData1 = reader.getUint8();
          qDebug("midi-2 %x %d %d", statusByte, event.mData0, event.mData1 );
          }
        }
      else {
        //Group data
        event.mType = groupStatusByte;
        if( (groupStatusByte & 0xf0) == 0xc0 || (groupStatusByte & 0xf0) == 0xd0 ) {
          //Programm change or Channel pressure
          event.mData0 = statusByte;
          event.mData1 = 0;
          qDebug("group midi-1 %x %d", event.mType, event.mData0 );
          }
        else {
          event.mData0 = statusByte;
          event.mData1 = reader.getUint8();
          qDebug("midi-2 %x %d %d", event.mType, event.mData0, event.mData1 );
          }
        }

      channelIndex = groupStatusByte & 0xf;
      mTracks[channelIndex].mEvents.append( event );
      }
    }
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

