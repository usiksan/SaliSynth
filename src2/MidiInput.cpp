/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
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
  hMidi(-1)
  {
  moveToThread( th );
  connect( th, &QThread::started, this, &MidiInput::onStart );
  }




MidiInput::~MidiInput()
  {
  if( hMidi >= 0 )
    close( hMidi );
  }





void MidiInput::onTimer()
  {
  char buf[30];
  int r, control, data[2], dataIndex = -1;
  do {
    //Read buffer
    r = read( hMidi, buf, 30 );

    //Parse bytes
    for( int i = 0; i < r; ++i ) {
      if( buf[i] & 0x80 ) {
        //Control byte i
        if( dataIndex >= 0 ) {
          emit midi( control, data[0], data[1] );
          qDebug() << "midi" << control << data[0] << data[1];
          }
        control = buf[i];
        dataIndex = 0;
        data[0] = data[1] = 0;
        }
      else {
        //Data byte
        if( dataIndex < 2 )
          data[dataIndex++] = buf[i];
        else
          dataIndex = -1;
        }
      }
    }
  while( r == 30 );
  if( dataIndex >= 0 ) {
    emit midi( control, data[0], data[1] );
    qDebug() << "midi" << control << data[0] << data[1];
    }
  }





void MidiInput::onStart()
  {
  QTimer *tm = new QTimer();
  tm->setInterval(10);
  hMidi = open( "/dev/snd/midiC1D0", O_RDONLY | O_NONBLOCK );
  if( hMidi >= 0 ) {
    connect( tm, &QTimer::timeout, this, &MidiInput::onTimer );
    tm->start();
    qDebug() << "midi open" << hMidi;
    }
  }


