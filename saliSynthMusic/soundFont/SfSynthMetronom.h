/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Metronom sound based on note. When construct, it creates sound wave for metronom.
*/
#ifndef SFSYNTHMETRONOM_H
#define SFSYNTHMETRONOM_H

#include "SfSynthNote.h"

class SfSynthMetronom : public SfSynthNote
  {
  public:
    SfSynthMetronom();
  };

#endif // SFSYNTHMETRONOM_H
