/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   SoundProcessor base class
*/
#ifndef SPBASE_H
#define SPBASE_H

#include "SoundProcessorManager.h"

class SpBase
  {
  public:
    SpBase();

    virtual void midi( quint8 cmd, quint8 data0, quint8 data1 );
  };

#endif // SPBASE_H
