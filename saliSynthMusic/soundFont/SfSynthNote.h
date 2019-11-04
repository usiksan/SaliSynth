/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Single note for synth. Note is single or set of sound played simultaneously.
   For this, note consist of one or more tracks each of that is separate sound
   generator.
   Note sounds when midi events.
*/
#ifndef SFSYNTHNOTE_H
#define SFSYNTHNOTE_H

#include "SfSynthTrack.h"
#include "SoundFont.h"

class SfSynthPreset;

//Note consist of one or more tracks
//Preset or program is 128 or less notes
class SfSynthNote
  {
    SfSynthTrackVector mTracks;  //Track of which consist a note
    int                mNote;    //Note index
    bool               mStopped; //Flag, true when no note sounds
  public:
    SfSynthNote();

    //Set note index
    void setNote( int note ) { mNote = note; }

    //Clear builded note to its default cleared nondefined state
    void clear();

    //Get next sound sample for this note
    int  sample();

    //Stop flag state
    bool isStopped() const { return mStopped; }

    void noteOff( quint8 pressure );

    bool noteOn( quint8 pressure );

    //Build track for note
    bool addTrack( quint16 *generator, const SfSample &sam, qint16 *samples, int endSample, int startLoop, int endLoop,
                   int delayVolEnv, int attackVolEnv, int holdVolEnv, int decayVolEnv, int releaseVolEnv );
  };

#endif // SFSYNTHNOTE_H
