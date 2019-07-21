/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SynthChannel.h"

SynthChannel::SynthChannel() :
  period(0)
  {

  }

void SynthChannel::noteOn(int noteIndex, int dynamic)
  {

  }

int *SynthChannel::prepare()
  {
  for( int i = 0; i < BUFFER_SIZE; i++ ) {
    if( period ) {
      if( i < 220 ) mBuffer[i*2+1] = 600;
      else mBuffer[i*2+1] = -600;
      }
    else {
      mBuffer[i*2+1] = 0;
      }
    mBuffer[i*2] = 0;
    }
  return mBuffer;
  }
