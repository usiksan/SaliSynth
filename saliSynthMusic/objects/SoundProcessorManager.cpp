#include "SoundProcessorManager.h"

SoundProcessorManager::SoundProcessorManager(QObject *parent) : QObject(parent)
  {

  }

void SoundProcessorManager::midiSlot(quint8 cmd, quint8 data0, quint8 data1)
  {
  emit midiSignal( cmd, data0, data1 );
  }
