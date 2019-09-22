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

    static void noteOff( int channel, int note );

    static void noteOn( int channel, int note, int pressure );

    static SoundPolyphonyChannel &channel( int channel, int note );

    static SoundPolyphonyChannel  mSoundPolyphony[POLYPHONY_COUNT];
  };

#endif // SOUNDPOLYPHONYMANAGER_H
