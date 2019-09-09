/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#ifndef SOUNDPOLYPHONYMANAGER_H
#define SOUNDPOLYPHONYMANAGER_H
#include "synthConfig.h"
#include "SoundPolyphonyChannel.h"

#include <QObject>

class SoundPolyphonyManager
  {
  public:

    static SoundPolyphonyChannel mSoundPolyphony[POLYPHONY_COUNT];
  };

#endif // SOUNDPOLYPHONYMANAGER_H
