/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SYNTH_H
#define SYNTH_H

#include <QObject>
#include <QThread>
#include <QAudioOutput>

#include "SynthChannel.h"


class Synth : public QObject
  {
    Q_OBJECT

    QAudioOutput *mAudioOutput;  //Audio PCM output channel
    QIODevice    *mOut;          //Pointer to write buffer
    SynthChannel  mChannels[16]; //16 MIDI channels
  public:
    explicit Synth( QThread *th, QObject *parent = nullptr);
    ~Synth();

  signals:

  public slots:
    void onInterval();

    void onMidi( int control, int data0, int data1 );
  private:
    void nextBuffer();
  };

#endif // SYNTH_H
