#include "MidiProcessor.h"

#include <QTimer>
#include <QDebug>




static inline bool isBlackNote( quint8 note ) { return note == noteAd || note == noteCd || note == noteDd || note == noteFd || note == noteGd; }

static inline bool isWhiteNote( quint8 note ) { return !isBlackNote(note); }


const quint8 noteMap[128] = {
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG, noteGd, noteA, noteAd, noteB, //12
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG, noteGd, noteA, noteAd, noteB, //24
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG, noteGd, noteA, noteAd, noteB, //36
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG, noteGd, noteA, noteAd, noteB, //48
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG, noteGd, noteA, noteAd, noteB, //60
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG, noteGd, noteA, noteAd, noteB, //72
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG, noteGd, noteA, noteAd, noteB, //84
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG, noteGd, noteA, noteAd, noteB, //96
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG, noteGd, noteA, noteAd, noteB, //108
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG, noteGd, noteA, noteAd, noteB, //120
  noteC, noteCd, noteD, noteDd, noteE, noteF, noteFd, noteG                                //127
};


const quint8 chordMap[chordCancel][5] = {
  { 0, 4, 7, 0, 0 }, //chordMaj
  { 0, 4, 7, 9, 0 }, //chordMaj6
  { 0, 4, 7,11, 0 }, //chordMaj7
  { 0, 4, 6, 7,11 }, //chordMaj7sharp11
  { 0, 2, 4, 7, 0 }, //chordMaj_9_
  { 0, 2, 4, 7,11 }, //chordMaj7_9_
  { 0, 2, 4, 7, 9 }, //chordMaj6_9_
  { 0, 4, 8, 0, 0 }, //chordAug
  { 0, 3, 7, 0, 0 }, //chordMin
  { 0, 3, 7, 9, 0 }, //chordMin6
  { 0, 3, 7,10, 0 }, //chordMin7
  { 0, 3, 6,10, 0 }, //chordMin7b5
  { 0, 2, 3, 7, 0 }, //chordMin_9_
  { 0, 2, 3, 7,10 }, //chordMin7_9_
  { 0, 3, 5, 7,10 }, //chordMin7_11_
  { 0, 3, 7,11, 0 }, //chordMinMaj7
  { 0, 2, 3, 7,11 }, //chordMinMaj7_9_
  { 0, 3, 6, 0, 0 }, //chordDim
  { 0, 3, 6, 9, 0 }, //chordDim7
  { 0, 4, 7,10, 0 }, //chord7th
  { 0, 5, 7,10, 0 }, //chord7sus4
  { 0, 4, 6,10, 0 }, //chord7b5
  { 0, 2, 4, 7,10 }, //chord7_9_
  { 0, 4, 6, 7,10 }, //chord7sharp11
  { 0, 4, 7, 9,10 }, //chord7_13_
  { 0, 1, 4, 7,10 }, //chord7_b9_
  { 0, 4, 7, 8,10 }, //chord7_b13_
  { 0, 3, 4, 7,10 }, //chord7_sharp9_
  { 0, 4, 8,11, 0 }, //chordMaj7aug
  { 0, 4, 8,10, 0 }, //chord7aug
  { 0, 0, 0, 0, 0 }, //chord1plus8
  { 0, 7, 0, 0, 0 }, //chord1plus5
  { 0, 5, 7, 0, 0 }, //chordSus4
  { 0, 2, 7, 0, 0 }  //chord1plus2plus5
};


static inline bool isBlackKey( quint8 code ) { return isBlackNote( noteMap[code & 0x7f] ); }

static inline bool isWhiteKey( quint8 code ) { return !isBlackKey( code ); }


MidiProcessor::MidiProcessor(QThread *th, QObject *parent) :
  QObject(parent),
  mLeftChordTimer(0)
  {
  for( int i = 0; i < 5; i++ ) mLeftChord[i] = 0;

  mQmlKeyboard = new QmlKeyboard();
  mQmlKeyboard->build( 61 );
  connect( mQmlKeyboard, &QmlKeyboard::keyEvent, this, &MidiProcessor::midiQmlKeyboard );
  connect( this, &MidiProcessor::keyIndicate, mQmlKeyboard, &QmlKeyboard::indicate );

  mQmlMidiFile = new QmlMidiFile();
  connect( mQmlMidiFile, &QmlMidiFile::midiEvent, this, &MidiProcessor::midiFile );
  connect( mQmlMidiFile, &QmlMidiFile::voiceSetup, this, &MidiProcessor::voiceSetup );

  mQmlStyleFile = new QmlStyleFile();
  connect( mQmlStyleFile, &QmlStyleFile::midiEvent, this, &MidiProcessor::midiFile );
  connect( mQmlStyleFile, &QmlStyleFile::voiceSetup, this, &MidiProcessor::voiceSetup );
  connect( this, &MidiProcessor::midiChord, mQmlStyleFile, &QmlStyleFile::chord );

  moveToThread( th );
  connect( th, &QThread::started, this, &MidiProcessor::onStart );
  }





void MidiProcessor::onStart()
  {
  QTimer *tm = new QTimer();
  tm->setInterval(10);
  connect( tm, &QTimer::timeout, this, &MidiProcessor::onTimer );
  tm->start();
  }



void MidiProcessor::midiQmlKeyboard(quint8 cmd, quint8 data0, quint8 data1)
  {
  if( (cmd & 0xf0) == 0xb0 && data0 == 7 )
    emit midiSignal( cmd, data0, data1 );
  else
    midiKeyboard( cmd, data0, data1 );
  }




void MidiProcessor::midiKeyboard(quint8 cmd, quint8 data0, quint8 data1)
  {
  if( (cmd == 120 || cmd == 126) && data0 == 0 && data1 == 0 )
    return;
  qDebug() << "midi" << cmd << data0 << data1;
  return;
  quint8 key = ((cmd >> 4) & 0x7);
  if( key == 0 || key == 1 ) {
    //This event key press or key release
//    if( cmd & 0x10 )
//      qDebug() << "midi" << cmd << data0 << data1;
    //qDebug() << "delimiter code" << mQmlKeyboard->delimiterCode();
    if( data0 < mQmlKeyboard->delimiterCode() ) keyboardLeft( cmd, data0, data1 );
    else                                        keyboardRight( cmd, data0, data1 );
    emit keyIndicate( data0, data1 != 0, 2 );
    }
//  else
//    emit midiSignal( cmd, data0, data1 );
  }



void MidiProcessor::midiFile(quint8 cmd, quint8 data0, quint8 data1)
  {
  //qDebug() << "midi file cmd" << cmd << data0 << data1;
  emit midiSignal( cmd, data0, data1 );
  }




void MidiProcessor::onTimer()
  {
  if( (mQmlKeyboard->leftMode() & QmlKeyboard::KmChordMask) == QmlKeyboard::KmChordActive ) keyboardLeftChordDetector();
  }



void MidiProcessor::keyboardLeft(quint8 cmd, quint8 data0, quint8 data1)
  {
  if( (mQmlKeyboard->leftMode() & QmlKeyboard::KmChordMask) == QmlKeyboard::KmChordActive )
    //Simple chord generator
    keyboardLeftSimpleChordGenerator( cmd, data0, data1 );
  else
    //Through play
    keyboardLeftOutput( (cmd & 0xf0) | 1, data0, data1 );
//  quint8 offset = data0 - mQmlKeyboard->leftCode();
  //  emit midiSignal( cmd + 1, offset + 60, data1 );
  }




void MidiProcessor::keyboardLeftOutput(quint8 cmd, quint8 data0, quint8 data1)
  {
  if( (mQmlKeyboard->leftMode() & QmlKeyboard::KmLeftMask) != QmlKeyboard::KmLeftAccomp )
    //Simple play
    emit midiSignal( cmd, data0, data1 );
  }




void MidiProcessor::keyboardLeftSimpleChordGenerator(quint8 cmd, qint8 data0, quint8 data1)
  {
  if( data1 && (cmd & 0x10) == 0x10 ) {
    //Press key
    mLeftChord[5] = data1;
    for( int i = 0; i < 5; i++ )
      if( mLeftChord[i] == 0 ) {
        mLeftChord[i] = data0;
        mLeftChordTimer = 5;
        break;
        }
    }
  else if( mLeftChord[0] || (cmd & 0x10) == 0 ) {
    //Release key
    if( (mQmlKeyboard->leftMode() & QmlKeyboard::KmLeftMask) == QmlKeyboard::KmLeftAccomp ) {
      for( int i = 0; i < 5; i++ ) {
        if( mLeftChord[i] )
          emit keyIndicate( mLeftChord[i], false, 0x4 );
        mLeftChord[i] = 0;
        }
      }
    else {
      //Voice mode
      for( int i = 0; i < 5; i++ ) {
        if( mLeftChord[i] ) {
          emit midiSignal( 0x11, mLeftChord[i], 0 );
          emit keyIndicate( mLeftChord[i], false, 0x4 );
          }
        mLeftChord[i] = 0;
        }
      }
    }
  }




void MidiProcessor::keyboardLeftChordDetector()
  {
  if( mLeftChordTimer && mLeftChord[0] ) {
    mLeftChordTimer--;
    if( mLeftChordTimer == 0 ) {
      //Chord detect
      int chordType;

      //Sort codes
      if( mLeftChord[0] < mLeftChord[1] ) qSwap( mLeftChord[0], mLeftChord[1] );
      if( mLeftChord[1] < mLeftChord[2] ) qSwap( mLeftChord[1], mLeftChord[2] );
      if( mLeftChord[0] < mLeftChord[1] ) qSwap( mLeftChord[0], mLeftChord[1] );

      if( mLeftChord[1] == 0 )          chordType = chordMaj;
      else {
        if( isBlackKey(mLeftChord[1]) ) chordType = chordMin;
        else                            chordType = chord7th;

        if( mLeftChord[2] )             chordType = chordMin7;
        }

      if( (mQmlKeyboard->leftMode() & QmlKeyboard::KmLeftMask) == QmlKeyboard::KmLeftAccomp ) {
        //Acompaniment mode
        emit midiChord( noteMap[ mLeftChord[0] ], chordType );
        //Indicate detected chord
        for( int i = 0; i < 5; i++ )
          if( i == 0 || chordMap[chordType][i] ) {
            mLeftChord[i] = chordMap[chordType][i] + mLeftChord[0];
            emit keyIndicate( mLeftChord[i], true, 0x4 );
            }
          else mLeftChord[i] = 0;
        }
      else {
        //Voice mode
        //Send detected chord
        for( int i = 0; i < 5; i++ )
          if( i == 0 || chordMap[chordType][i] ) {
            mLeftChord[i] = chordMap[chordType][i] + mLeftChord[0];
            keyboardLeftOutput( 0x11, mLeftChord[i], mLeftChord[5] );
            emit keyIndicate( mLeftChord[i], true, 0x4 );
            }
          else mLeftChord[i] = 0;
        }
      }
    }
  }




void MidiProcessor::keyboardRight(quint8 cmd, quint8 data0, quint8 data1)
  {
  emit midiSignal( cmd, data0, data1 );
  if( (mQmlKeyboard->rightMode() & QmlKeyboard::KmRightMask) == QmlKeyboard::KmRightOverlay )
    //Duplicate voice on second channel
    emit midiSignal( (cmd & 0xf0) | 2, data0, data1 );
  }
