/*
Project "MIDI synth"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "Synth.h"
#include <QAudioFormat>
#include <QDebug>

Synth::Synth(QThread *th, QObject *parent) :
  QObject(parent),
  mAudioOutput(nullptr),
  mOut(nullptr)
  {
  QAudioFormat format;
  // Set up the format, eg.
  format.setSampleRate(44100);
  format.setChannelCount(2);
  format.setSampleSize(16);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);

  QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
  if( !info.isFormatSupported(format) ) {
    qWarning() << "Raw audio format not supported by backend, cannot play audio.";
    return;
    }

  mAudioOutput = new QAudioOutput( format );
  //Buffer size = SynthChannel buffer * 2 channels (stereo) * 2 bytes per sample * 4 frames
  mAudioOutput->setBufferSize( BUFFER_SIZE*2*2*8 );
  mAudioOutput->setNotifyInterval( 10 );

  mOut = mAudioOutput->start();
  moveToThread( th );
  mAudioOutput->moveToThread( th );

  nextBuffer();
  nextBuffer();

  connect( mAudioOutput, &QAudioOutput::notify, this, &Synth::onInterval );

  mChannels[0].setPeriod(1);
  qDebug() << "audio output" << mAudioOutput->bufferSize() << mAudioOutput->notifyInterval();
  }




Synth::~Synth()
  {
  if( mAudioOutput ) {
    mAudioOutput->stop();
    delete mAudioOutput;
    }
  }




void Synth::onInterval()
  {
  qDebug() << "onInterval" << mAudioOutput->bytesFree();
  while( mAudioOutput->bytesFree() > (BUFFER_SIZE*2*2*8 - BUFFER_SIZE*2*2*7)  )
    nextBuffer();
  }





void Synth::onMidi(int control, int data0, int data1)
  {
  int channel = control & 0xf;
  switch( control & 0x70 ) {
    case 0x00 : mChannels[channel].noteOff( data0, data1 );         break;
    case 0x10 : mChannels[channel].noteOn( data0, data1 );          break;
    case 0x20 : mChannels[channel].polyphonicKey( data0, data1 );   break;
    case 0x30 : mChannels[channel].controlChange( data0, data1 );   break;
    case 0x40 : mChannels[channel].programmChange( data0 );         break;
    case 0x50 : mChannels[channel].channelPressure( data0 );        break;
    case 0x60 : mChannels[channel].pichWheelChange( data0, data1 ); break;
    }
  }




typedef int *intptr;
void Synth::nextBuffer()
  {
  qDebug() << "nextBuffer" << mAudioOutput->bytesFree();
  qint16 buffer[BUFFER_SIZE*2];
  //Prepare next channels buffers
  //pointers to channel buffers
  intptr ptr[16];
  for( int i = 0; i < 16; i++ )
    ptr[i] = mChannels[i].prepare();
  //Mix channels
  for( int i = 0; i < BUFFER_SIZE; i++ ) {
    int left = *(ptr[0])++ + *(ptr[1])++ + *(ptr[2])++ + *(ptr[3])++ +
               *(ptr[4])++ + *(ptr[5])++ + *(ptr[6])++ + *(ptr[7])++ +
               *(ptr[8])++ + *(ptr[9])++ + *(ptr[10])++ + *(ptr[11])++ +
               *(ptr[12])++ + *(ptr[13])++ + *(ptr[14])++ + *(ptr[15])++;
    int right = *(ptr[0])++ + *(ptr[1])++ + *(ptr[2])++ + *(ptr[3])++ +
               *(ptr[4])++ + *(ptr[5])++ + *(ptr[6])++ + *(ptr[7])++ +
               *(ptr[8])++ + *(ptr[9])++ + *(ptr[10])++ + *(ptr[11])++ +
               *(ptr[12])++ + *(ptr[13])++ + *(ptr[14])++ + *(ptr[15])++;
    //average
    if( left < -32768 )  left = -32768;
    if( left > 32767 )   left = 32767;
    if( right < -32768 ) right = -32768;
    if( right > 32767 )  right = 32767;
    buffer[i * 2 + 1] = left;
    buffer[i * 2] = right;
    }
  //Buffer completed
  //write it
  mOut->write( (const char *)buffer, BUFFER_SIZE * 2 * 2 );
  }


