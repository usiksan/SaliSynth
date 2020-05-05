#include "QmlStyleFile.h"

#include <QDebug>

QmlStyleFile::QmlStyleFile(QObject *parent) :
  QmlMidiFile(parent)
  {

  }

int QmlStyleFile::parts() const
  {
  int mask = 0;
  switch( mPart ) {
    case 0 :
      if( mMarkerSet.contains( QStringLiteral("Intro A") ) ) mask |= spIntroA;
      if( mMarkerSet.contains( QStringLiteral("Intro B") ) ) mask |= spIntroB;
      if( mMarkerSet.contains( QStringLiteral("Intro C") ) ) mask |= spIntroC;
      if( mMarkerSet.contains( QStringLiteral("Intro D") ) ) mask |= spIntroD;
      break;

    case spMainA :
      if( mMarkerSet.contains( QStringLiteral("Fill In AA")) ) mask |= spMainA;
      if( mMarkerSet.contains( QStringLiteral("Fill In AB")) ) mask |= spMainB;
      if( mMarkerSet.contains( QStringLiteral("Fill In AC")) ) mask |= spMainC;
      if( mMarkerSet.contains( QStringLiteral("Fill In AD")) ) mask |= spMainD;
      if( mMarkerSet.contains( QStringLiteral("Ending A")) )   mask |= spEndingA;
      break;

    case spMainB :
      if( mMarkerSet.contains( QStringLiteral("Fill In BA")) ) mask |= spMainA;
      if( mMarkerSet.contains( QStringLiteral("Fill In BB")) ) mask |= spMainB;
      if( mMarkerSet.contains( QStringLiteral("Fill In BC")) ) mask |= spMainC;
      if( mMarkerSet.contains( QStringLiteral("Fill In BD")) ) mask |= spMainD;
      if( mMarkerSet.contains( QStringLiteral("Ending B")) )   mask |= spEndingB;
      break;

    case spMainC :
      if( mMarkerSet.contains( QStringLiteral("Fill In CA")) ) mask |= spMainA;
      if( mMarkerSet.contains( QStringLiteral("Fill In CB")) ) mask |= spMainB;
      if( mMarkerSet.contains( QStringLiteral("Fill In CC")) ) mask |= spMainC;
      if( mMarkerSet.contains( QStringLiteral("Fill In CD")) ) mask |= spMainD;
      if( mMarkerSet.contains( QStringLiteral("Ending C")) )   mask |= spEndingC;
      break;

    case spMainD :
      if( mMarkerSet.contains( QStringLiteral("Fill In DA")) ) mask |= spMainA;
      if( mMarkerSet.contains( QStringLiteral("Fill In DB")) ) mask |= spMainB;
      if( mMarkerSet.contains( QStringLiteral("Fill In DC")) ) mask |= spMainC;
      if( mMarkerSet.contains( QStringLiteral("Fill In DD")) ) mask |= spMainD;
      if( mMarkerSet.contains( QStringLiteral("Ending D")) )   mask |= spEndingD;
      break;
    }

  return mask;
  }



void QmlStyleFile::playPart(int part)
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
  mGroupList.clear();

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
  StyleGroup group;
  while( !reader.isEnd() ) {
    IffReader track = reader.getChunk();
    qDebug() << "CSEG reader chunk" << track.name();
    if( track.compareChunkName("Sdec") ) {
      readSdec( track, group );
      }
    else if( track.compareChunkName("Ctab") ) {
      readCtab( track, group );
      }
    }

  //Append group to the list
  mGroupList.append( group );
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



void QmlStyleFile::readSdec( IffReader &reader, StyleGroup &group )
  {
  //Read string with segments list
  char str[1024];
  auto len = qMin(reader.chunkLenght(),1023u);
  reader.readChars( str, len );
  str[len] = 0;

  //Split string to string list with segments list names
  QStringList list = QString::fromLatin1(str).split( QChar(',') );

  //All names from segments list insert to the set of markers
  for( auto s : list )
    group.mMarkerSet.insert(s);

  qDebug() << "Sdec line" << str << group.mMarkerSet;
  }




void QmlStyleFile::readCtab( IffReader &reader, StyleGroup &group )
  {
  StyleTrack t;
  t.mSrcChannel = reader.getUint8();
  qDebug() << "source channel" << t.mSrcChannel;

  reader.readChars( t.mName, 8 );
  t.mName[8] = 0;
  qDebug() << "name" << t.mName;

  t.mDstChannel = reader.getUint8();
  qDebug() << "dest" << t.mDstChannel;

  t.mEditable = reader.getUint8();
  qDebug() << "editable" << t.mEditable;

  t.mNoteMuteMask = reader.getUint16be();
  qDebug() << "note mute mask" << t.mNoteMuteMask;

  t.mChordMute = reader.getUint8();

  t.mChordMute <<= 32;
  t.mChordMute |= reader.getUint32be();
  qDebug() << "chord mute full" << t.mChordMute;

  t.mSourceChord = reader.getUint8();
  qDebug() << "source chord" << t.mSourceChord;

  t.mSourceChordType = reader.getUint8();
  qDebug() << "source chord type" << t.mSourceChordType;

  t.mNoteTranspositionRules = reader.getUint8();
  qDebug() << "note transposition rules" << t.mNoteTranspositionRules;

  t.mNoteTranspositionTable = reader.getUint8();
  qDebug() << "note transposition table" << t.mNoteTranspositionTable;

  t.mHighKey = reader.getUint8();
  qDebug() << "high key" << t.mHighKey;

  t.mNoteLowLimit = reader.getUint8();
  qDebug() << "note low limit" << t.mNoteLowLimit;

  t.mNoteHighLimit = reader.getUint8();
  qDebug() << "note high limit" << t.mNoteHighLimit;

  t.mTriggerRule = reader.getUint8();
  qDebug() << "retrigger rule" << t.mTriggerRule;

  int specialFeatures = reader.getUint8();
  qDebug() << "special features" << specialFeatures;

  group.mTrackList.append( t );
  }




void QmlStyleFile::postRead()
  {
  for( const auto marker : mMarkerList )
    mMarkerSet.insert( marker.mMarker );
  emit partsChanged();
  }
