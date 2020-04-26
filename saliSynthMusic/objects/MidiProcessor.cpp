#include "MidiProcessor.h"

#include <QTimer>
#include <QDebug>


MidiProcessor::MidiProcessor(QThread *th, QObject *parent) :
  QObject(parent)
  {
  mQmlKeyboard = new QmlKeyboard();
  mQmlKeyboard->build( 61 );
  connect( mQmlKeyboard, &QmlKeyboard::keyEvent, this, &MidiProcessor::midiKeyboard );
  connect( this, &MidiProcessor::keyIndicate, mQmlKeyboard, &QmlKeyboard::indicate );

  mQmlMidiFile = new QmlMidiFile();

  moveToThread( th );
  connect( th, &QThread::started, this, &MidiProcessor::onStart );
  }





void MidiProcessor::onStart()
  {
  QTimer *tm = new QTimer();
  tm->setInterval(10);
  connect( tm, &QTimer::timeout, this, &MidiProcessor::onTimer );
  tm->start();
  }




void MidiProcessor::midiKeyboard(quint8 cmd, quint8 data0, quint8 data1)
  {
  quint8 key = ((cmd >> 4) & 0x7);
  if( key == 0 || key == 1 ) {
    //This event key press or key release
    //qDebug() << "delimiter code" << mQmlKeyboard->delimiterCode();
    if( data0 < mQmlKeyboard->delimiterCode() ) keyboardLeft( cmd, data0, data1 );
    else                                        keyboardRight( cmd, data0, data1 );
    emit keyIndicate( data0, data1 != 0, 2 );
    }
  else
    emit midiSignal( cmd, data0, data1 );
  }



void MidiProcessor::midiFile(quint8 cmd, quint8 data0, quint8 data1)
  {

  }




void MidiProcessor::onTimer()
  {

  }



void MidiProcessor::keyboardLeft(quint8 cmd, quint8 data0, quint8 data1)
  {
  quint8 offset = data0 - mQmlKeyboard->leftCode();
  emit midiSignal( cmd, offset + 60, data1 );
  }


void MidiProcessor::keyboardRight(quint8 cmd, quint8 data0, quint8 data1)
  {
  emit midiSignal( cmd, data0, data1 );
  }
