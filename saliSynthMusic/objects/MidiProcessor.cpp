#include "MidiProcessor.h"

#include <QTimer>
#include <QDebug>



enum Notes {
  noteA,
  noteAd,
  noteB,
  noteC,
  noteCd,
  noteD,
  noteDd,
  noteE,
  noteF,
  noteFd,
  noteG,
  noteGd };

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

enum Chords {
  chordMajor,
  chordMinor,
  chord7,
  chordm7,
  chordM7,
  chordLast
  };

const quint8 chordMap[chordLast][4] = {
  { 0, 4, 7, 0 }, //chordMajor
  { 0, 3, 7, 0 }, //chordMinor
  { 0, 4, 7, 10 }, //chord7
  { 0, 3, 7, 10 }, //chordm7
  { 0, 4, 7, 11 }  //chordM7
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
  connect( mQmlKeyboard, &QmlKeyboard::keyEvent, this, &MidiProcessor::midiKeyboard );
  connect( this, &MidiProcessor::keyIndicate, mQmlKeyboard, &QmlKeyboard::indicate );

  mQmlMidiFile = new QmlMidiFile();

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




void MidiProcessor::midiKeyboard(quint8 cmd, quint8 data0, quint8 data1)
  {
  quint8 key = ((cmd >> 4) & 0x7);
  if( key == 0 || key == 1 ) {
    //This event key press or key release
    //qDebug() << "delimiter code" << mQmlKeyboard->delimiterCode();
    if( data0 < mQmlKeyboard->delimiterCode() ) keyboardLeft( cmd, data0, data1 );
    else                                        keyboardRight( cmd, data0, data1 );
    emit keyIndicate( data0, data1 != 0, 2 );
    }
  else
    emit midiSignal( cmd, data0, data1 );
  }



void MidiProcessor::midiFile(quint8 cmd, quint8 data0, quint8 data1)
  {

  }




void MidiProcessor::onTimer()
  {
  if( mQmlKeyboard->leftMode() & 1 ) keyboardLeftChordDetector();
  }



void MidiProcessor::keyboardLeft(quint8 cmd, quint8 data0, quint8 data1)
  {
  static bool init = false;
  if( !init ) {
    init = true;
    emit midiSignal( 0x41, 0, 0 );
    }
  if( mQmlKeyboard->leftMode() & 1 )
    //Simple chord generator
    keyboardLeftSimpleChordGenerator( cmd, data0, data1 );
  else
    //Through play
    keyboardLeftOutput( cmd, data0, data1 );
//  quint8 offset = data0 - mQmlKeyboard->leftCode();
  //  emit midiSignal( cmd + 1, offset + 60, data1 );
  }




void MidiProcessor::keyboardLeftOutput(quint8 cmd, quint8 data0, quint8 data1)
  {
  if( mQmlKeyboard->leftMode() & 2 )
    //Accompaniment
    ;
  else
    //Simple play
    emit midiSignal( cmd, data0, data1 );
  }




void MidiProcessor::keyboardLeftSimpleChordGenerator(quint8 cmd, qint8 data0, quint8 data1)
  {
  if( data1 && (cmd & 0x10) == 0x10 ) {
    //Press key
    mLeftChord[4] = data1;
    for( int i = 0; i < 3; i++ )
      if( mLeftChord[i] == 0 ) {
        mLeftChord[i] = data0;
        mLeftChordTimer = 3;
        break;
        }
    }
  else if( mLeftChord[0] || (cmd & 0x10) == 0 ) {
    //Release key
    for( int i = 0; i < 4; i++ ) {
      if( mLeftChord[i] ) {
        emit midiSignal( 0x11, mLeftChord[i], 0 );
        emit keyIndicate( mLeftChord[i], false, 0x4 );
        }
      mLeftChord[i] = 0;
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

      if( mLeftChord[1] == 0 )          chordType = chordMajor;
      else {
        if( isBlackKey(mLeftChord[1]) ) chordType = chordMinor;
        else                            chordType = chord7;

        if( mLeftChord[2] )             chordType = chordm7;
        }

      //Send detected chord
      for( int i = 0; i < 4; i++ )
        if( i == 0 || chordMap[chordType][i] ) {
          mLeftChord[i] = chordMap[chordType][i] + mLeftChord[0];
          keyboardLeftOutput( 0x11, mLeftChord[i], mLeftChord[4] );
          emit keyIndicate( mLeftChord[i], true, 0x4 );
          }
        else mLeftChord[i] = 0;
      }
    }
  }


void MidiProcessor::keyboardRight(quint8 cmd, quint8 data0, quint8 data1)
  {
  emit midiSignal( cmd, data0, data1 );
  }
