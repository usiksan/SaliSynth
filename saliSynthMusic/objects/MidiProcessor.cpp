#include "MidiProcessor.h"

#include <QTimer>


MidiProcessor::MidiProcessor(QThread *th, QObject *parent) :
  QObject(parent)
  {
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
  emit midiSignal( cmd, data0, data1 );
  }




void MidiProcessor::onTimer()
  {

  }
