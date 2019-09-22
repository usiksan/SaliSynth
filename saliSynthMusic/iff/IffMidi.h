/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   MIDI file
*/
#ifndef IFFMIDI_H
#define IFFMIDI_H

#include "IffReader.h"

#include <QString>

class IffMidi
  {
  public:
    IffMidi();

    bool read( QString fname );

  private:
    bool readMthd( IffReader &reader );
    void readMtrk( IffReader &reader );
  };

#endif // IFFMIDI_H
