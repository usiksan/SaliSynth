/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#ifndef SOUNDMIDICHANNEL_H
#define SOUNDMIDICHANNEL_H

#include <QString>

class SoundMidiChannel
  {
  public:
    SoundMidiChannel();

    void midi( quint8 status, quint8 data0, quint8 data1 );
  };

#endif // SOUNDMIDICHANNEL_H
