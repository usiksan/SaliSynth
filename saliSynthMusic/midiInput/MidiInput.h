/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   MIDI device input (MIDI keyboard)
*/
#ifndef MIDIINPUT_H
#define MIDIINPUT_H

#include <QObject>
#include <QThread>
#include <QFile>

class MidiInput : public QObject
  {
    Q_OBJECT

    int    mMidiHandle; //Handle to midi keyboard
    int    mDataIndex;
    //Midi message
    quint8 mControl;
    quint8 mData0;
  public:
    explicit MidiInput( QThread *th, QObject *parent = nullptr);
    ~MidiInput();

  signals:
    void midiSignal( quint8 control, quint8 data0, quint8 data1 );

    //Emit when connect or disconnect keyboard
    void connectionChanged( bool connected );

  public slots:
    void onTimer();
    void onStart();
  };

#endif // MIDIINPUT_H
