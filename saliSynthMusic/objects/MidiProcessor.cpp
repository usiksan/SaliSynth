#include "MidiProcessor.h"

#include <QTimer>


MidiProcessor::MidiProcessor(QThread *th, QObject *parent) :
  QObject(parent),
  mKeyDelimiter(62)
  {
  mQmlKeyboard = new QmlKeyboard();
  mQmlKeyboard->build( 61 );
  connect( mQmlKeyboard, &QmlKeyboard::keyEvent, this, &MidiProcessor::midiSlot );
  connect( this, &MidiProcessor::keyIndicate, mQmlKeyboard, &QmlKeyboard::indicate );

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




void MidiProcessor::midiSlot(quint8 cmd, quint8 data0, quint8 data1)
  {
  quint8 key = ((cmd >> 4) & 0x7);
  if( key == 0 || key == 1 ) {
    //This event key press or key release
    if( data0 < mKeyDelimiter ) keyboardLeft( cmd, data0, data1 );
    else                        keyboardRight( cmd, data0, data1 );
    emit keyIndicate( data0, data1 != 0, 2 );
    }
  else
    emit midiSignal( cmd, data0, data1 );
  }




void MidiProcessor::onTimer()
  {

  }

void MidiProcessor::keyboardLeft(quint8 cmd, quint8 data0, quint8 data1)
  {

  }

void MidiProcessor::keyboardRight(quint8 cmd, quint8 data0, quint8 data1)
  {
  emit midiSignal( cmd, data0, data1 );
  }
