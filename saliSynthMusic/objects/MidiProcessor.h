#ifndef MIDIPROCESSOR_H
#define MIDIPROCESSOR_H

#include "QmlKeyboard.h"
#include "QmlMidiFile.h"

#include <QObject>
#include <QThread>


class MidiProcessor: public QObject
  {
    Q_OBJECT

    QmlKeyboard *mQmlKeyboard;   //! Keyboard representation on the screen
    QmlMidiFile *mQmlMidiFile;   //! QML midi file representation

  public:
    MidiProcessor( QThread *th, QObject *parent = nullptr );

    //!
    //! \brief emitMidi Emits signal with one midi command. Used
    //! \param cmd      MIDI command
    //! \param data0    MIDI command data 0
    //! \param data1    MIDI command data 1
    //!
    void midiEmit( quint8 cmd, quint8 data0, quint8 data1 );

    QmlKeyboard *qmlKeyboard() const { return mQmlKeyboard; }

    QmlMidiFile *qmlMidiFile() const { return mQmlMidiFile; }
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


    void keyIndicate( quint8 keyCode, bool set, quint8 colorMask );

  public slots:
    void onStart();

    void midiKeyboard( quint8 cmd, quint8 data0, quint8 data1 );

    void midiFile( quint8 cmd, quint8 data0, quint8 data1 );

  private slots:
    void onTimer();

  private:
    void keyboardLeft( quint8 cmd, quint8 data0, quint8 data1 );
    void keyboardRight( quint8 cmd, quint8 data0, quint8 data1 );
  };

#endif // MIDIPROCESSOR_H
