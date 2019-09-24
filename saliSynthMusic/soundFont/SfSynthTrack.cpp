#include "SfSynthTrack.h"

#include <math.h>
#include <QDebug>

int SfSynthTrack::mAttenuator[1024];

SfSynthTrack::SfSynthTrack() :
  mSamples(nullptr),     //Samples vector
  mSampleEnd(0),
  mSampleLoopStart(0),
  mSampleLoopEnd(0),
  mExitLoop(0),        //Walk through end loop, else looping
  //Oscillator
  mSampleIndex(0),     //Current sample index
  mSampleSubIndex(0),  //Fractional part of sample index in 1/65536
  mSampleStep(0),      //Step of sample to provide a given sample rate. In 1/65536
  //Amplitude envelope modulator
  mVolumeInitial(0),      //In santiBell note default attenuation
  mVolume(0),             //Normal signal volume without regulation. In 1/32768
  mVolumePhase(vpStop),
  mVolumeTick(0),
  mAttenuation(0),        //Current attenuation level. At phase attack - linear multer in 1/32768 of volume
  mAttenuationStep(0),    //At attack phase it is linear step to increase result volume from zero to mVolume. In 1/32768 of volume
  mVolDelayEnvelope(0),   //Delay before sound start. In tick
  mVolAttackEnvelope(0),  //Attack time. Volume rich from zero to peak. In tick
  mVolHoldEnvelope(0),    //Hold time. Volume hold peak level
  mVolDecayEnvelope(0),   //Time for volume linearly ramps toward the sustain level
  mVolReleaseEnvelope(0), //Time
  mVolSustainLevel(0)    //This is the decrease in level, expressed in centibels, to which the Volume Envelope value ramps during the decay phase.
  {
  if( !mAttenuator[0] ) {
    //Fill attenuator table
    for( int i = 0; i < 1024; i++ ) {
      double sb = i; //santiBell
      mAttenuator[i] = static_cast<int>( 1.0 / pow( 10.0, sb / 200.0 ) * 32768.0 );
      qDebug() << i << mAttenuator[i];
      }
    }
  }


void SfSynthTrack::setup(const qint16 *samples, int sampleEnd, int sampleLoopStart, int sampleLoopEnd, bool exitLoop, int sampleStep, int volumeInitial, int volDelayEnvelope, int volAttackEnvelope, int volHoldEnvelope, int volDecayEnvelope, int volReleaseEnvelope, int volSustainLevel)
  {
  mSamples = samples;     //Samples vector
  mSampleEnd = sampleEnd;
  mSampleLoopStart = sampleLoopStart;
  mSampleLoopEnd = sampleLoopEnd;
  mExitLoop = exitLoop;        //Walk through end loop, else looping
  //Oscillator
  mSampleIndex = 0;     //Current sample index
  mSampleSubIndex = 0;  //Fractional part of sample index in 1/65536
  mSampleStep = sampleStep;      //Step of sample to provide a given sample rate. In 1/65536
  //Amplitude envelope modulator
  mVolumeInitial = volumeInitial;      //In santiBell note default attenuation
  mVolume = 0;             //Normal signal volume without regulation. In 1/32768
  mVolumePhase = vpStop;
  mVolumeTick = 0;
  mAttenuation = 0;        //Current attenuation level. At phase attack - linear multer in 1/32768 of volume
  mAttenuationStep = 0;    //At attack phase it is linear step to increase result volume from zero to mVolume. In 1/32768 of volume
  mVolDelayEnvelope = volDelayEnvelope;   //Delay before sound start. In tick
  mVolAttackEnvelope = volAttackEnvelope;  //Attack time. Volume rich from zero to peak. In tick
  mVolHoldEnvelope = volHoldEnvelope;    //Hold time. Volume hold peak level
  mVolDecayEnvelope = volDecayEnvelope;   //Time for volume linearly ramps toward the sustain level
  mVolReleaseEnvelope = volReleaseEnvelope; //Time
  mVolSustainLevel = volSustainLevel;    //This is the decrease in level, expressed in centibels, to which the Volume Envelope value ramps during the decay phase.
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
      mAttenuation = 0;
      if( mVolAttackEnvelope < 64 )
        mAttenuationStep = mVolume << 15;
      else
        mAttenuationStep = (mVolume << 15) / (mVolAttackEnvelope >> 6);
      }
    return 0;
    }

  if( mVolumePhase == vpAttack ) {
    if( mVolumeTick >= mVolAttackEnvelope ) {
      //Attack phase completed
      mVolumeTick = 0;
      mVolumePhase = vpHold;
      mAttenuation = 0;
      return nextSample() * mVolume >> 15;
      }
    if( (mVolumeTick & 0x3f) == 0 )
      mAttenuation += mAttenuationStep;
    return nextSample() * (mAttenuation >> 15) >> 15;
    }

  if( mVolumePhase == vpHold ) {
    if( mVolumeTick >= mVolHoldEnvelope ) {
      //Hold phase completed
      mVolumeTick = 0;
      mVolumePhase = vpDecay;
      mAttenuation = 0;
      if( mVolDecayEnvelope < 64 )
        mAttenuationStep = mVolSustainLevel << 15;
      else
        mAttenuationStep = (mVolSustainLevel << 15) / (mVolDecayEnvelope >> 6);
      }
    return nextSample() * mVolume >> 15;
    }

  if( mVolumePhase == vpDecay ) {
    if( (mVolumeTick & 0x3f) == 0 ) {
      mAttenuation += mAttenuationStep;
      //If in decay phase we rich -100dB then stop track
      if( mAttenuation >= (999 << 15) ) {
        mVolumePhase = vpStop;
        return 0;
        }
      }
    if( mVolumeTick >= mVolDecayEnvelope ) {
      //Decay phase completed
      mVolumePhase = vpSustain;
      mAttenuation = mVolSustainLevel << 15;
      }
    return nextSample() * ( mVolume * mAttenuator[(mAttenuation >> 15) & 0x3ff] >> 15 ) >> 15;
    }

  if( mVolumePhase == vpSustain )
    return nextSample() * ( mVolume * mAttenuator[mVolSustainLevel] >> 15 ) >> 15;

  if( mVolumePhase == vpRelease ) {
    if( (mVolumeTick & 0x3f) == 0 )
      mAttenuation += mAttenuationStep;
    if( mVolumeTick >= mVolReleaseEnvelope )
      mVolumePhase = vpStop;
    return nextSample() * ( mVolume * mAttenuator[(mAttenuation >> 15) & 0x3ff] >> 15 ) >> 15;
    }

  return 0;
  }




void SfSynthTrack::noteOff(quint8 pressure)
  {
  Q_UNUSED(pressure)
  switch( mVolumePhase ) {
    case vpStop :
    case vpDelay :
      mVolumePhase = vpStop;
      break;
    case vpAttack :
    case vpHold :
      mAttenuation = 0;
    [[fallthrough]];
    case vpDecay :
    case vpSustain :
    case vpRelease :
      //Start release phase
      if( mAttenuation > (1000 << 15) ) mAttenuation = (1000 << 15);
      mAttenuationStep = ((1000 << 15) - mAttenuation) / (mVolReleaseEnvelope >> 6);
      mVolumeTick = 0;
      mVolumePhase = vpRelease;
      break;
    }
  }



bool SfSynthTrack::noteOn(quint8 pressure)
  {
  if( pressure == 0 ) noteOff(0);
  else {
    mVolumeTick = 0;
    mSampleSubIndex = 0;
    mSampleIndex = 0;
    mVolume = pressure * mAttenuator[mVolumeInitial] / 127;
    mVolumePhase = vpDelay;
    }
  return mVolumePhase != vpStop;
  }




int SfSynthTrack::nextSample()
  {
  //Calculate sample value as linear interpolation between neighboring samples on mSampleSubIndex value
  int curSample  = sampleAtIndex();
  int delta = sampleAtNextIndex() - curSample;
  curSample += delta * mSampleSubIndex >> 16;

  //Calculate next sample index
  mSampleSubIndex += mSampleStep;
  mSampleIndex += mSampleSubIndex >> 16; //Add integer part of index to sampleIndex
  mSampleSubIndex &= 0xffff; //Leave only fractional part

  if( mExitLoop ) {
    if( mSampleIndex + 1 >= mSampleEnd ) mVolumePhase = vpStop;
    }
  else {
    if( mSampleIndex >= mSampleLoopEnd ) {
      mSampleIndex -= mSampleLoopEnd;
      mSampleIndex += mSampleLoopStart;
      }
    }

  return curSample;
  }
