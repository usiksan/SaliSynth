/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   MIDI file
*/
#include "MidiFile.h"

#include <QDebug>

MidiFile::MidiFile()
  {

  }





bool MidiFile::read(QString fname)
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








bool MidiFile::readMthd(IffReader &reader)
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







void MidiFile::readMtrk(IffReader &reader)
  {
  quint32 time = 0;
  QString trackName;
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

      if( metaEvent == 3 )
        //Track name
        trackName = QString::fromUtf8( ar );

      else if( metaEvent == 0x51 ) {
        //Set tempo
        mTempo = static_cast<quint32>(
                   (static_cast<unsigned>(ar[0] & 0xff) << 16) |
                   (static_cast<unsigned>(ar[1] & 0xff) << 8) |
                   static_cast<unsigned>(ar[2] & 0xff) );
        }

      else if( metaEvent == 0x58 ) {
        //Time signature

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
      event.mType = statusByte;
      if( (statusByte & 0xf0) == 0xc0 || (statusByte & 0xf0) == 0xd0 ) {
        //Programm change or Channel pressure
        event.mData0 = reader.getUint8();
        qDebug("midi-1 %x %d", statusByte, event.mData0 );
        }
      else {
        event.mData0 = reader.getUint8();
        event.mData1 = reader.getUint8();
        qDebug("midi-2 %x %d %d", statusByte, event.mData0, event.mData1 );
        }
      int trackIndex = statusByte & 0xf;
      if( !trackName.isEmpty() ) {
        mTracks[trackIndex].mName = trackName;
        trackName.clear();
        }
      mTracks[trackIndex].mEvents.append( event );
      }
    }
  }





quint32 MidiFile::variableLenValue(IffReader &reader)
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
