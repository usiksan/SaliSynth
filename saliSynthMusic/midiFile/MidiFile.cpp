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
  while( !reader.isEnd() ) {
    IffReader track = reader.getChunk();
    qDebug() << track.name();
    if( track.compareChunkName("MTrk") )
      readMtrk( track );
    }
  return true;
  }

bool MidiFile::readMthd(IffReader &reader)
  {
  qDebug() << reader.name();
  mFormat      = reader.getUint16be();
  mTrackNumber = reader.getUint16be();
  mDivision    = reader.getUint16be();

  qDebug() << mFormat << mTrackNumber << mDivision;
  return true;
  }

void MidiFile::readMtrk(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    //Read midi event

    quint32 timeOffset = variableLenValue(reader);
    quint8 statusByte = reader.getUint8();
    qDebug() << timeOffset << statusByte;
    if( statusByte == 0xff ) {
      //Meta-event
      quint8 metaEvent = reader.getUint8();
      quint32 len = variableLenValue(reader);
      QByteArray ar( static_cast<int>(len), 0 );
      for( quint32 i = 0; i < len; i++ )
        ar[i] = reader.getInt8();
      qDebug() << "meta" << metaEvent << len << ar;
      }
    else if( statusByte == 0xf0 ) {
      //Sys-ex
      quint32 len = variableLenValue(reader);
      QByteArray ar( static_cast<int>(len), 0 );
      for( quint32 i = 0; i < len; i++ )
        ar[i] = reader.getInt8();
      qDebug() << "sys-ex" << len << ar;
      }
    else {
      //Midi-event
      quint8 data0 = reader.peekUint8();
      if( data0 & 0x80 ) {
        qDebug() << "midi-0";

        }
      else {
        data0 = reader.getUint8();
        quint8 data1 = reader.peekUint8();
        if( data1 & 0x80 ) {
          qDebug() << "midi-1" << data0;
          }
        else {
          data1 = reader.getUint8();
          qDebug() << "midi-2" << data0 << data1;
          }
        }
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
