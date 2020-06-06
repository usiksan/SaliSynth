/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Style. Style is midi file with CASM extension. This style supports Yamaha style files
*/
#include "QmlStyleFile.h"

#include <QDebug>

QmlStyleFile::QmlStyleFile(QObject *parent) :
  QmlMidiFile(parent),
  mCurPart(0),
  mMainPart(0)
  {

  }

int QmlStyleFile::parts() const
  {
  int mask = 0;
  if( mMarkerSet.contains( QStringLiteral("Intro A") ) ) mask |= spIntroA;
  if( mMarkerSet.contains( QStringLiteral("Intro B") ) ) mask |= spIntroB;
  if( mMarkerSet.contains( QStringLiteral("Intro C") ) ) mask |= spIntroC;
  if( mMarkerSet.contains( QStringLiteral("Intro D") ) ) mask |= spIntroD;
  if( mMarkerSet.contains( QStringLiteral("Main A")) )   mask |= spMainA;
  if( mMarkerSet.contains( QStringLiteral("Main B")) )   mask |= spMainB;
  if( mMarkerSet.contains( QStringLiteral("Main C")) )   mask |= spMainC;
  if( mMarkerSet.contains( QStringLiteral("Main D")) )   mask |= spMainD;
  if( mMarkerSet.contains( QStringLiteral("Fill In AA")) ) mask |= spFillA;
  if( mMarkerSet.contains( QStringLiteral("Ending A")) )   mask |= spEndingA;
  if( mMarkerSet.contains( QStringLiteral("Fill In BB")) ) mask |= spFillB;
  if( mMarkerSet.contains( QStringLiteral("Ending B")) )   mask |= spEndingB;
  if( mMarkerSet.contains( QStringLiteral("Fill In CC")) ) mask |= spFillC;
  if( mMarkerSet.contains( QStringLiteral("Ending C")) )   mask |= spEndingC;
  if( mMarkerSet.contains( QStringLiteral("Fill In DD")) ) mask |= spFillD;
  if( mMarkerSet.contains( QStringLiteral("Ending D")) )   mask |= spEndingD;
  return mask;
  }



void QmlStyleFile::playPart(int part)
  {
  if( mCurPart == 0 ) {
    //Setup all voices
    for( int i = 0; i < mQmlTrackModel.count(); i++ ) {
      int trackIndex = mQmlTrackModel.asInt( i, TRACK_INDEX );
      emit voiceSetup( mQmlTrack[trackIndex].channel(), mQmlTrackModel.asInt( i, TRACK_ID ) );
      }

    //Play desired intro
    switch( part ) {
      case spIntroA :
        addPart( part, QStringLiteral("Intro A"), false );
        addMainPart( mMainPart );
        break;
      case spIntroB :
        addPart( part, QStringLiteral("Intro B"), false );
        addMainPart( mMainPart );
        break;
      case spIntroC :
        addPart( part, QStringLiteral("Intro C"), false );
        addMainPart( mMainPart );
        break;
      case spIntroD :
        addPart( part, QStringLiteral("Intro D"), false );
        addMainPart( mMainPart );
        break;

      case spMainA :
      case spMainB :
      case spMainC :
      case spMainD :
        //Simple switch main part
        mMainPart = part;
        emit mainPartChanged();
        break;
      }
    }
  else {
    //When playing any part
    switch( part ) {
      case spMainA :
        if( mMainPart == spMainA )
          addPart( spFillA, QStringLiteral("Fill in AA"), false );
        addMainPart( mMainPart = part );
        emit mainPartChanged();
        break;

      case spMainB :
        if( mMainPart == spMainB )
          addPart( spFillB, QStringLiteral("Fill in BB"), false );
        addMainPart( mMainPart = part );
        emit mainPartChanged();
        break;

      case spMainC :
        if( mMainPart == spMainC )
          addPart( spFillC, QStringLiteral("Fill in CC"), false );
        addMainPart( mMainPart = part );
        emit mainPartChanged();
        break;

      case spMainD :
        if( mMainPart == spMainD )
          addPart( spFillD, QStringLiteral("Fill in DD"), false );
        addMainPart( mMainPart = part );
        emit mainPartChanged();
        break;

      case spEndingA :
        if( mCurPart & (spMainA | spMainB | spMainC | spMainD) )
          addPart( part, QStringLiteral("Ending A"), false );
        break;

      case spEndingB :
        if( mCurPart & (spMainA | spMainB | spMainC | spMainD) )
          addPart( part, QStringLiteral("Ending B"), false );
        break;

      case spEndingC :
        if( mCurPart & (spMainA | spMainB | spMainC | spMainD) )
          addPart( part, QStringLiteral("Ending C"), false );
        break;

      case spEndingD :
        if( mCurPart & (spMainA | spMainB | spMainC | spMainD) )
          addPart( part, QStringLiteral("Ending D"), false );
        break;
      }
    }
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
        //Style track
        qint8 shift(0);
        if( mLoop.head().mGroup != nullptr && mLoop.head().mGroup->mTrackMap.contains(trackIndex) ) {
          const StyleTrack &st( mLoop.head().mGroup->mTrackMap.value(trackIndex) );
          if( st.mNoteTranspositionTable )
            shift = mNote - st.mSourceChord;
          }
        mQmlTrack[trackIndex].tick( mTickCount >> 4, nextTime >> 4, soundOn, volume, shift );
        }

      //Update current tick count
      mTickCount = nextTime;

      if( tickCount() >= mLoop.head().mTimeStop ) {
        //This is last time, wrap if loop
        if( mLoop.count() > 1 ) {
          //Jump to next part
          mLoop.dequeue();
          setTickCount( mLoop.head().mTimeStart );
          mCurPart = mLoop.head().mPartMask;
          emit curPartChanged();
          }
        else if( mLoop.head().mLoop ) {
          //Wrap fragment
          setTickCount( mLoop.head().mTimeStart );
          }
        else {
          mTickCount = -1;
          mLoop.dequeue();
          mCurPart = 0;
          emit curPartChanged();
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
    mCurPart = mLoop.head().mPartMask;
    emit curPartChanged();
    mNote = 0;
    mChordType = 0;
    }
  }




void QmlStyleFile::stop()
  {
  //Clear style queue
  mLoop.clear();
  QmlMidiFile::stop();
  }




void QmlStyleFile::chord(quint8 note, quint8 chordType)
  {
  mNote = note;
  mChordType = chordType;
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

  group.mTrackMap.insert( t.mSrcChannel, t );
  }




void QmlStyleFile::postRead()
  {
  for( const auto marker : mMarkerList )
    mMarkerSet.insert( marker.mMarker );
  mCurPart = 0;
  mMainPart = spMainA;
  emit curPartChanged();
  emit mainPartChanged();
  emit partsChanged();
  }




void QmlStyleFile::addPart( int partMask, const QString part, bool loop)
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

      //Setup style looping
      styleLoop.mLoop = loop;

      //Setup style part mask
      styleLoop.mPartMask = partMask;

      //Find group for this part
      styleLoop.mGroup = nullptr; //Group not foune yet
      for( auto &g : mGroupList )
        if( g.mMarkerSet.contains(part) ) {
          //Group found
          styleLoop.mGroup = &g;
          break;
          }

      //Append to loop queue
      mLoop.enqueue( styleLoop );
      return;
      }
  }



void QmlStyleFile::addMainPart(int mainPart)
  {
  switch( mainPart ) {
    case spMainA :
      addPart( mainPart, QStringLiteral("Main A"), true );
      mMainPart = mainPart;
      break;
    case spMainB :
      addPart( mainPart, QStringLiteral("Main B"), true );
      mMainPart = mainPart;
      break;
    case spMainC :
      addPart( mainPart, QStringLiteral("Main C"), true );
      mMainPart = mainPart;
      break;
    case spMainD :
      addPart( mainPart, QStringLiteral("Main D"), true );
      mMainPart = mainPart;
      break;
    }
  emit mainPartChanged();
  }




