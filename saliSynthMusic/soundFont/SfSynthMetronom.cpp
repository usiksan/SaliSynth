/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Metronom sound based on note. When construct, it creates sound wave for metronom.
*/
#include "SfSynthMetronom.h"

//Metronom wave table
static const qint16 metronomWave[72] = {
  0,2000,4000,6000,8000,10000,12000,14000,
  16000,20000,24000,30000,30000,24000,20000,16000,
  14000,12000,10000,8000,6000,4000,2000,0,
  0,-2000,-4000,-6000,-8000,-10000,-12000,-14000,
  -16000,-20000,-24000,-30000,-30000,-24000,-20000,-16000,
  -14000,-12000,-10000,-8000,-6000,-4000,-2000,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};



SfSynthMetronom::SfSynthMetronom()
  {
  //Build track with metronom sound
  SfSynthTrack track;
  track.setup( metronomWave, 72, 0, 72, true,
               128000,//219,
               0,
               0, 0, 0, 0, 0,
               0, 0x7f00
               );
  mTracks.append( track );
  }
