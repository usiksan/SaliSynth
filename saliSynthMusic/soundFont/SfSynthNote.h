#ifndef SFSYNTHNOTE_H
#define SFSYNTHNOTE_H

#include "SfSynthTrack.h"

class SfSynthPreset;

//Note consist of one or more tracks
//Preset or program is 128 or less notes
class SfSynthNote
  {
    SfSynthTrackVector mTracks; //Track of which consist a note
    bool               mStopped;
  public:
    SfSynthNote();

    void clear();

    int  sample();

    bool isStopped() const { return mStopped; }

    void noteOff( quint8 pressure );

    bool noteOn( quint8 pressure );
  };

#endif // SFSYNTHNOTE_H
