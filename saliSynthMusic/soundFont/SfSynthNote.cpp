#include "SfSynthNote.h"

SfSynthNote::SfSynthNote() :
  mStopped(true)
  {

  }

void SfSynthNote::clear()
  {
  mStopped = true;
  }

int SfSynthNote::sample()
  {
  if( !mStopped && mTracks.count() ) {
    //Calculate sum of all tracks
    int sampleSum = 0;
    bool stopped = true;
    for( SfSynthTrack &track : mTracks )
      sampleSum += track.sample( stopped );
    mStopped = stopped;
    return sampleSum;
    }
  return 0;
  }


void SfSynthNote::noteOff(quint8 pressure)
  {
  for( SfSynthTrack &track : mTracks )
    track.noteOff( pressure );
  }



bool SfSynthNote::noteOn(quint8 pressure)
  {
  bool started = false;
  for( SfSynthTrack &track : mTracks )
    started = track.noteOn( pressure ) || started;
  mStopped = !started;
  return started;
  }



