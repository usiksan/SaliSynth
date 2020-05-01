#include "QmlStyleFile.h"

#include <QDebug>

QmlStyleFile::QmlStyleFile(QObject *parent) :
  QmlMidiFile(parent)
  {

  }


void QmlStyleFile::readExtension(IffReader &reader)
  {
  if( reader.compareChunkName("CASM") )
    readCASM( reader );
  else if( reader.compareChunkName("OTSc") )
    readOTS_OTSc( reader );
  else if( reader.compareChunkName("FNRc") )
    readMDB_FNRc( reader );
  else if( reader.compareChunkName("MHhd") )
    readMH_MHhd( reader );
  }




void QmlStyleFile::readCASM(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    IffReader track = reader.getChunk();
    qDebug() << "CASM reader chunk" << track.name();
    if( track.compareChunkName("CSEG") ) {
      readCSEG( track );
      }
    }
  }




void QmlStyleFile::readCSEG(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    IffReader track = reader.getChunk();
    qDebug() << "CSEG reader chunk" << track.name();
    if( track.compareChunkName("Sdec") ) {
      readSdec( track );
      }
    else if( track.compareChunkName("Ctab") ) {
      readCtab( track );
      }
    }
  }



void QmlStyleFile::readOTS_OTSc(IffReader &reader)
  {

  }

void QmlStyleFile::readMDB_FNRc(IffReader &reader)
  {

  }

void QmlStyleFile::readMH_MHhd(IffReader &reader)
  {

  }



void QmlStyleFile::readSdec(IffReader &reader)
  {
  //String
  char str[1024];
  auto len = qMin(reader.chunkLenght(),1023u);
  reader.readChars( str, len );
  str[len] = 0;
  qDebug() << "Sdec line" << str;
  }



void QmlStyleFile::readCtab(IffReader &reader)
  {
  int srcChannel = reader.getUint8();
  qDebug() << "source channel" << srcChannel;

  char name[9];
  reader.readChars( name, 8 );
  name[8] = 0;
  qDebug() << "name" << name;

  int dstChannel = reader.getUint8();
  qDebug() << "dest" << dstChannel;

  int editable = reader.getUint8();
  qDebug() << "editable" << editable;

  int noteMuteMask = reader.getUint16be();
  qDebug() << "note mute mask" << noteMuteMask;

  int chordMute0 = reader.getUint8();
  qDebug() << "chord mute 0" << chordMute0;

  quint32 chordMute1 = reader.getUint32be();
  qDebug() << "chord mute 1" << chordMute1;

  int sourceChord = reader.getUint8();
  qDebug() << "source chord" << sourceChord;

  int sourceChordType = reader.getUint8();
  qDebug() << "source chord type" << sourceChordType;

  int noteTranspositionRules = reader.getUint8();
  qDebug() << "note transposition rules" << noteTranspositionRules;

  int noteTranspositionTable = reader.getUint8();
  qDebug() << "note transposition table" << noteTranspositionTable;

  int highKey = reader.getUint8();
  qDebug() << "high key" << highKey;

  int noteLowLimit = reader.getUint8();
  qDebug() << "note low limit" << noteLowLimit;

  int noteHighLimit = reader.getUint8();
  qDebug() << "note high limit" << noteHighLimit;

  int retriggerRule = reader.getUint8();
  qDebug() << "retrigger rule" << retriggerRule;

  int specialFeatures = reader.getUint8();
  qDebug() << "special features" << specialFeatures;

  }


