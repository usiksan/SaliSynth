#include "SfSynthTrack.h"

SfSynthTrack::SfSynthTrack()
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
