/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#ifndef MIDIPROCESSOR_H
#define MIDIPROCESSOR_H

#include "QmlKeyboard.h"
#include "QmlMidiFile.h"
#include "QmlStyleFile.h"

#include <QObject>
#include <QThread>

enum Notes {
  noteC,
  noteCd,
  noteD,
  noteDd,
  noteE,
  noteF,
  noteFd,
  noteG,
  noteGd,
  noteA,
  noteAd,
  noteB
  };


enum Chords {
  chordMaj,          //0,4,7
  chordMaj6,         //0,4,7,9
  chordMaj7,         //0,4,7,11
  chordMaj7sharp11,  //0,4,6,7,11
  chordMaj_9_,       //0,2,4,7
  chordMaj7_9_,      //0,2,4,7,11
  chordMaj6_9_,      //0,2,4,7,9
  chordAug,          //0,4,8
  chordMin,          //0,3,7
  chordMin6,         //0,3,7,9
  chordMin7,         //0,3,7,10
  chordMin7b5,       //0,3,6,10
  chordMin_9_,       //0,2,3,7
  chordMin7_9_,      //0,2,3,7,10
  chordMin7_11_,     //0,3,5,7,10
  chordMinMaj7,      //0,3,7,11
  chordMinMaj7_9_,   //0,2,3,7,11
  chordDim,          //0,3,6
  chordDim7,         //0,3,6,9
  chord7th,          //0,4,7,10
  chord7sus4,        //0,5,7,10
  chord7b5,          //0,4,6,10
  chord7_9_,         //0,2,4,7,10
  chord7sharp11,     //0,4,6,7,10
  chord7_13_,        //0,4,7,9,10
  chord7_b9_,        //0,1,4,7,10
  chord7_b13_,       //0,4,7,8,10
  chord7_sharp9_,    //0,3,4,7,10
  chordMaj7aug,      //0,4,8,11
  chord7aug,         //0,4,8,10
  chord1plus8,       //0
  chord1plus5,       //0,7
  chordSus4,         //0,5,7
  chord1plus2plus5,  //0,2,7
  chordCancel
  };


class MidiProcessor: public QObject
  {
    Q_OBJECT

    QmlKeyboard  *mQmlKeyboard;    //! Keyboard representation on the screen
    QmlMidiFile  *mQmlMidiFile;    //! QML midi file representation
    QmlStyleFile *mQmlStyleFile;   //! QML autoacompaniment style file representation

    quint8        mLeftChord[6];   //! 0-4 chord, 5-pressure
    int           mLeftChordTimer;
  public:
    MidiProcessor( QThread *th, QObject *parent = nullptr );

    //!
    //! \brief emitMidi Emits signal with one midi command. Used
    //! \param cmd      MIDI command
    //! \param data0    MIDI command data 0
    //! \param data1    MIDI command data 1
    //!
    void midiEmit( quint8 cmd, quint8 data0, quint8 data1 );

    QmlKeyboard  *qmlKeyboard() const { return mQmlKeyboard; }

    QmlMidiFile  *qmlMidiFile() const { return mQmlMidiFile; }

    QmlStyleFile *qmlStyleFile() const { return mQmlStyleFile; }
  signals:
    void qmlKeyboardChanged();

    //!
    //! \brief midi  MIDI export. This signal emits one midi command.
    //!              This signal connected to the wave synth and generate real sound
    //! \param cmd   MIDI command
    //! \param data0 MIDI command data 0
    //! \param data1 MIDI command data 1
    //!
    void midiSignal( quint8 cmd, quint8 data0, quint8 data1 );

    //!
    //! \brief voiceSetup Set specify voice on channel
    //! \param channel    Channel on which need to setup voice
    //! \param voiceId    Voice ident which need to set to channel
    //!
    void voiceSetup( quint8 channel, int voiceId );

    void keyIndicate( quint8 keyCode, bool set, quint8 colorMask );

    void midiChord( quint8 chordBase, quint8 chordType );

  public slots:
    void onStart();

    void midiKeyboard( quint8 cmd, quint8 data0, quint8 data1 );

    void midiFile( quint8 cmd, quint8 data0, quint8 data1 );

  private slots:
    void onTimer();

  private:
    void keyboardLeft( quint8 cmd, quint8 data0, quint8 data1 );
    void keyboardLeftOutput( quint8 cmd, quint8 data0, quint8 data1 );
    void keyboardLeftSimpleChordGenerator( quint8 cmd, qint8 data0, quint8 data1 );
    void keyboardLeftChordDetector();
    void keyboardRight( quint8 cmd, quint8 data0, quint8 data1 );
  };

#endif // MIDIPROCESSOR_H
