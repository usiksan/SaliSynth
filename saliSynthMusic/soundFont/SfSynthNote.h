#ifndef SFSYNTHNOTE_H
#define SFSYNTHNOTE_H

#include "SfSynthTrack.h"
#include "SoundFont.h"

class SfSynthPreset;

//Note consist of one or more tracks
//Preset or program is 128 or less notes
class SfSynthNote
  {
    SfSynthTrackVector mTracks; //Track of which consist a note
    int                mNote;
    bool               mStopped;
  public:
    SfSynthNote();

    void setNote( int note ) { mNote = note; }

    void clear();

    int  sample();

    bool isStopped() const { return mStopped; }

    void noteOff( quint8 pressure );

    bool noteOn( quint8 pressure );

    bool addTrack( quint16 *generator, const SfSample &sam, qint16 *samples, int endSample, int startLoop, int endLoop,
                   int delayVolEnv, int attackVolEnv, int holdVolEnv, int decayVolEnv, int releaseVolEnv );
  };

#endif // SFSYNTHNOTE_H
