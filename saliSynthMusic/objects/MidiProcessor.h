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


class MidiProcessor: public QObject
  {
    Q_OBJECT

    QmlKeyboard  *mQmlKeyboard;    //! Keyboard representation on the screen
    QmlMidiFile  *mQmlMidiFile;    //! QML midi file representation
    QmlStyleFile *mQmlStyleFile;   //! QML autoacompaniment style file representation

    quint8        mLeftChord[5];   //! 0-3 chord, 4-pressure
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
