/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#ifndef SOUNDBUFFERIODEVICE_H
#define SOUNDBUFFERIODEVICE_H

#include "synthConfig.h"

#include <QIODevice>
#include <QElapsedTimer>


class SoundBufferIODevice : public QIODevice
  {
    Q_OBJECT

    QElapsedTimer mTimer;
  public:
    SoundBufferIODevice();

    // QIODevice interface
  public:
    virtual bool isSequential() const override;
    virtual qint64 bytesAvailable() const override;

  protected:
    virtual qint64 readData(char *data, qint64 maxlen) override;
    virtual qint64 writeData(const char *data, qint64 len) override;
  };

#endif // SOUNDBUFFERIODEVICE_H
