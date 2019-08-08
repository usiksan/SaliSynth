/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#ifndef SOUNDPOLYPHONYCHANNEL_H
#define SOUNDPOLYPHONYCHANNEL_H


class SoundPolyphonyChannel
  {
    int period;
    int current;
  public:
    SoundPolyphonyChannel();

    //Return next channel sample
    int sample();

    void setPeriod( int p ) { period = p; }
  };



#endif // SOUNDPOLYPHONYCHANNEL_H
