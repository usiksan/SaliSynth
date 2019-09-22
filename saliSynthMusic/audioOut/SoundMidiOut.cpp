/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#include "SoundMidiOut.h"
#include "SoundPolyphonyManager.h"

#include <QDebug>

SoundMidiOut::SoundMidiOut(QObject *parent) : QObject(parent)
  {

  }

void SoundMidiOut::midi(quint8 status, quint8 data0, quint8 data1)
  {
  int channel = status & 0xf;
  int cmd = ((status >> 4) & 0x7);
  switch( cmd ) {
    case 0 :
      //Note off [note pressure]
      SoundPolyphonyManager::noteOff( channel, data0 );
      break;
    case 1 :
      //Note on [note pressure]
      SoundPolyphonyManager::noteOn( channel, data0, data1 );
      break;
    case 2 :
      //Polyphonic key pressure [note pressure]
      break;
    case 3 :
      //Control change (controller number) (controller value)
      break;
    case 4 :
      //Programm change (programm number)
      break;
    case 5 :
      //Channel pressure (pressure)
      break;
    case 6 :
      //Pitch wheel change (lsb) (msb)
      break;

    }
  if( cmd != 7 )
    qDebug() << "channel " << channel << "cmd " << cmd << data0 << data1;
  }
