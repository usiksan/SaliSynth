/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#include "SoundBufferIODevice.h"
#include "SoundPolyphonyManager.h"
#include <QDebug>
#include <QThread>
#include <QAudioOutput>

extern QAudioOutput *audio;

static int audioBufferSize;

SoundBufferIODevice::SoundBufferIODevice() :
  QIODevice()
  {
  mTimer.start();
  }




bool SoundBufferIODevice::isSequential() const
  {
  return true;
  }




static int fillBuffer = 0;

qint64 SoundBufferIODevice::bytesAvailable() const
  {
  if( fillBuffer <= 14 || (audioBufferSize - audio->bytesFree()) < SAMPLES_PER_20MS * 4  )
    return SAMPLES_PER_20MS * 2;
  return 0;
  }





qint64 SoundBufferIODevice::readData(char *data, qint64 maxlen)
  {
//  if( audioBufferSize == 0 )
//    audioBufferSize = audio->bufferSize();

//  if( fillBuffer > 14 && (audioBufferSize - audio->bytesFree()) >= SAMPLES_PER_20MS )
//    return 0;

  if( fillBuffer++ < 40 ) {
    qDebug()  << "maxlen" << maxlen << "bytes free" << audio->bytesFree() << "timer" << mTimer.restart();
    }


  qint16 *outSamples = static_cast<qint16*>( static_cast<void*>(data) );

  //Fill next samples
  for( int i = 0; i < SAMPLES_PER_20MS; i++ ) {
    //Summ all channels
    int sample = 0;
    for( int k = 0; k < POLYPHONY_COUNT; k++ )
      sample += SoundPolyphonyManager::mSoundPolyphony[k].sample();

    //Average sample
    if( sample > 32767 ) sample = 32767;
    if( sample < -32768 ) sample = -32768;

    //Store sample to output buffer
    outSamples[i] = static_cast<qint16>(sample);
    }

  return SAMPLES_PER_20MS * 2;
  }





qint64 SoundBufferIODevice::writeData(const char *data, qint64 len)
  {
  Q_UNUSED(data);
  Q_UNUSED(len);
  return 0;
  }
