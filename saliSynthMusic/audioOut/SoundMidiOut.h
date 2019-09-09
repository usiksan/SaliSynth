/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#ifndef SOUNDMIDIOUT_H
#define SOUNDMIDIOUT_H

#include "SoundMidiChannel.h"

#include <QObject>

class SoundMidiOut : public QObject
  {
    Q_OBJECT

    SoundMidiChannel mChannels[16];
  public:
    explicit SoundMidiOut(QObject *parent = nullptr);

  signals:

  public slots:
    void midi( quint8 status, quint8 data0, quint8 data1 );

  };

#endif // SOUNDMIDIOUT_H
