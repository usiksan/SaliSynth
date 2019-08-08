/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#include "SoundPolyphonyChannel.h"

SoundPolyphonyChannel::SoundPolyphonyChannel() :
  period(0),
  current(0)
  {

  }

int SoundPolyphonyChannel::sample()
  {
  if( period == 0 ) return 0;
  if( current >= period ) current = 0;
  if( current++ < period / 2 ) return -10000;
  else return 10000;
  }
