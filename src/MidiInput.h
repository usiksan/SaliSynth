/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef MIDIINPUT_H
#define MIDIINPUT_H

#include <QObject>
#include <QThread>
#include <QFile>

class MidiInput : public QObject
  {
    Q_OBJECT

    int hMidi; //Handle to midi keyboard
    //Midi message
    int mControl;
    int mData0;
    int mData1;
  public:
    explicit MidiInput( QThread *th, QObject *parent = nullptr);
    ~MidiInput();

  signals:
    void midi( int control, int data0, int data1 );

  public slots:
    void onTimer();
    void onStart();
  };

#endif // MIDIINPUT_H
