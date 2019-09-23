#include "SfSynthTrack.h"

SfSynthTrack::SfSynthTrack()
  {

  }

int SfSynthTrack::sample(bool &stopped)
  {
  if( mVolumePhase == vpStop )
    return mVolumeTick = 0;
  stopped = false;
  mVolumeTick++;
  if( mVolumePhase == vpDelay ) {
    if( mVolumeTick >= mVolDelayEnvelope ) {
      //Delay phase completed
      mVolumeTick = 0;
      mVolumePhase = vpAttack;
      }
    }
  }

void SfSynthTrack::noteOff(quint8 pressure)
  {

  }

bool SfSynthTrack::noteOn(quint8 pressure)
  {

  }




int SfSynthTrack::nextSample()
  {
  if( !mRun ) return 0;

  //Calculate sample value as linear interpolation between neighboring samples on mSampleSubIndex value
  int curSample  = sampleAtIndex();
  int delta = sampleAtNextIndex() - curSample;
  curSample += delta * mSampleSubIndex >> 16;

  //Calculate next sample index
  mSampleSubIndex += mSampleStep;
  mSampleIndex += mSampleSubIndex >> 16; //Add integer part of index to sampleIndex
  mSampleSubIndex &= 0xffff; //Leave only fractional part

  if( mExitLoop ) {
    if( mSampleIndex + 1 >= mSampleEnd ) mRun = false;
    }
  else {
    if( mSampleIndex >= mSampleLoopEnd ) {
      mSampleIndex -= mSampleLoopEnd;
      mSampleIndex += mSampleLoopStart;
      }
    }

  return curSample;
  }
