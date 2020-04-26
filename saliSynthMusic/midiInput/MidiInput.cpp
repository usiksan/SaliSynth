/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   MIDI device input (MIDI keyboard)
*/
#include "MidiInput.h"
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>



MidiInput::MidiInput(QThread *th, QObject *parent) :
  QObject(parent),
  mMidiHandle(-1),
  mDataIndex(-1)
  {
  moveToThread( th );
  connect( th, &QThread::started, this, &MidiInput::onStart );
  }




MidiInput::~MidiInput()
  {
  if( mMidiHandle >= 0 )
    close( mMidiHandle );
  }





void MidiInput::onTimer()
  {
  char buf[30];
  ssize_t r;
  do {
    //Read buffer
    r = read( mMidiHandle, buf, 30 );

    //Parse bytes
    for( int i = 0; i < r; ++i ) {
      if( buf[i] & 0x80 ) {
        //Control byte i
        if( mDataIndex >= 0 ) {
          emit midiSignal( mControl, mData0, 0 );
          //qDebug() << "before control midi" << mControl << mData0;
          }
        mControl = buf[i] & 0x7f;
        mDataIndex = 0;
        mData0 = 0;
        }
      else {
        //Data byte
        if( mDataIndex == 0 ) {
          mData0 = buf[i] & 0x7f;
          mDataIndex = 1;
          }
        else if( mDataIndex == 1 ) {
          emit midiSignal( mControl, mData0, buf[i] & 0x7f );
          qDebug() << "normal midi" << mControl << mData0 << static_cast<int>(buf[i] & 0x7f);
          mDataIndex = -1;
          }
        }
      }
    }
  while( r == 30 );
  if( mDataIndex >= 0 ) {
    emit midiSignal( mControl, mData0, 0 );
    //qDebug() << "post midi" << mControl << mData0;
    }
  }





void MidiInput::onStart()
  {
  mMidiHandle = open( "/dev/snd/midiC1D0", O_RDONLY | O_NONBLOCK );
  if( mMidiHandle >= 0 ) {
    QTimer *tm = new QTimer();
    tm->setInterval(10);
    connect( tm, &QTimer::timeout, this, &MidiInput::onTimer );
    tm->start();
    emit connectionChanged( true );
    qDebug() << "midi open" << mMidiHandle;
    }
  else {
    //Try connect after one second
    QTimer::singleShot( 1000, this, &MidiInput::onStart );
    }
  }


