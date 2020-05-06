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
  switch( part ) {
    case spIntroA :
      addPart( QStringLiteral("Intro A"), false );
      addPart( QStringLiteral("Main A"), true );
      mPart = spMainA;
      break;
    case spMainA :
      switch( mPart ) {
        case spMainA :
          addPart( QStringLiteral("Fill In AA"), false );
          break;
        case spMainB :
          addPart( QStringLiteral("Fill In BA"), false );
          break;
        case spMainC :
          addPart( QStringLiteral("Fill In CA"), false );
          break;
        case spMainD :
          addPart( QStringLiteral("Fill In DA"), false );
          break;
        }
      addPart( QStringLiteral("Main A"), true );
      mPart = spMainA;
      break;
    case spEndingA :
      addPart( QStringLiteral("Ending A"), false );
      mPart = 0;
      break;

    case spIntroB :
      addPart( QStringLiteral("Intro B"), false );
      addPart( QStringLiteral("Main B"), true );
      mPart = spMainB;
      break;
    case spMainB :
      switch( mPart ) {
        case spMainA :
          addPart( QStringLiteral("Fill In AB"), false );
          break;
        case spMainB :
          addPart( QStringLiteral("Fill In BB"), false );
          break;
        case spMainC :
          addPart( QStringLiteral("Fill In CB"), false );
          break;
        case spMainD :
          addPart( QStringLiteral("Fill In DB"), false );
          break;
        }
      addPart( QStringLiteral("Main B"), true );
      mPart = spMainB;
      break;
    case spEndingB :
      addPart( QStringLiteral("Ending B"), false );
      mPart = 0;
      break;

    case spIntroC :
      addPart( QStringLiteral("Intro C"), false );
      addPart( QStringLiteral("Main C"), true );
      mPart = spMainC;
      break;
    case spMainC :
      switch( mPart ) {
        case spMainA :
          addPart( QStringLiteral("Fill In AC"), false );
          break;
        case spMainB :
          addPart( QStringLiteral("Fill In BC"), false );
          break;
        case spMainC :
          addPart( QStringLiteral("Fill In CC"), false );
          break;
        case spMainD :
          addPart( QStringLiteral("Fill In DC"), false );
          break;
        }
      addPart( QStringLiteral("Main C"), true );
      mPart = spMainC;
      break;
    case spEndingC :
      addPart( QStringLiteral("Ending C"), false );
      mPart = 0;
      break;

    case spIntroD :
      addPart( QStringLiteral("Intro D"), false );
      addPart( QStringLiteral("Main D"), true );
      mPart = spMainD;
      break;
    case spMainD :
      switch( mPart ) {
        case spMainA :
          addPart( QStringLiteral("Fill In AD"), false );
          break;
        case spMainB :
          addPart( QStringLiteral("Fill In BD"), false );
          break;
        case spMainC :
          addPart( QStringLiteral("Fill In CD"), false );
          break;
        case spMainD :
          addPart( QStringLiteral("Fill In DD"), false );
          break;
        }
      addPart( QStringLiteral("Main D"), true );
      mPart = spMainD;
      break;
    case spEndingD :
      addPart( QStringLiteral("Ending D"), false );
      mPart = 0;
      break;
    }
  emit partsChanged();
  }




void QmlStyleFile::tick()
  {
  if( mTickCount >= 0 ) {
    //We need send event beatween startTime and lastTime
    // startTime is mTickCount, and lastTime - is nextTime
    //At first we calculate nextTime
    int nextTime = mTickCount + mTickStep;

    if( (nextTime >> 4) > mLoop.head().mTimeStop )
      nextTime = mLoop.head().mTimeStop << 4;

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

      if( tickCount() >= mLoop.head().mTimeStop ) {
        //This is last time, wrap if loop
        if( mLoop.count() > 1 ) {
          //Jump to next part
          mLoop.dequeue();
          setTickCount( mLoop.head().mTimeStart );
          }
        else if( mLoop.head().mLoop ) {
          //Wrap fragment
          setTickCount( mLoop.head().mTimeStart );
          }
        else {
          mTickCount = -1;
          mLoop.dequeue();
          }
        }
      //Signal that tick count changed
      //On this signal visual elements display actual playing position
      emit tickCountChanged();
      }
    else
      //Update current tick count
      mTickCount = nextTime;
    }
  else if( mLoop.count() ) {
    setTickCount( mLoop.head().mTimeStart );
    }
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
  mPart = 0;
  emit partsChanged();
  }




void QmlStyleFile::addPart(const QString part, bool loop)
  {
  int i = 0;
  for( i = 0; i < mMarkerList.count(); i++ )
    if( mMarkerList.at(i).mMarker == part ) {
      //Marker found
      StyleLoop styleLoop;
      styleLoop.mTimeStart = mMarkerList.at(i).mTime;
      if( i + 1 < mMarkerList.count() )
        styleLoop.mTimeStop = mMarkerList.at(i + 1).mTime;
      else
        styleLoop.mTimeStop = mFileLenght;
      styleLoop.mLoop = loop;
      mLoop.enqueue( styleLoop );
      return;
      }
  }




