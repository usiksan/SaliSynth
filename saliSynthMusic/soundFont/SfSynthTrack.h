#ifndef SFSYNTHTRACK_H
#define SFSYNTHTRACK_H

#include <QVector>
#include <stdlib.h>

#define dB 1.12201845430196

//Synth one track from one instrument
class SfSynthTrack
  {
    const qint16 *mSamples;     //Samples vector
    int           mSampleEnd;
    int           mSampleLoopStart;
    int           mSampleLoopEnd;
    bool          mExitLoop;        //Walk through end loop, else looping

    //Oscillator
    int           mSampleIndex;     //Current sample index
    int           mSampleSubIndex;  //Fractional part of sample index in 1/65536
    int           mSampleStep;      //Step of sample to provide a given sample rate. In 1/65536

    //Amplitude envelope modulator
    int           mVolumeInitial;      //In santiBell note default attenuation
    int           mVolume;             //Normal signal volume without regulation. In 1/32768
    enum {
      vpStop,
      vpDelay,
      vpAttack,
      vpHold,
      vpDecay,
      vpSustain,
      vpRelease,
      }           mVolumePhase;
    int           mVolumeTick;
    int           mAttenuation;        //Current attenuation level. At phase attack - linear multer in 1/32768 of volume
                                       //At other phases - db in 1/32768 of santiBell
    int           mAttenuationStep;    //At attack phase it is linear step to increase result volume from zero to mVolume. In 1/32768 of volume
                                       //At decay and release phases - it linear step to decrease volume from current level to target level. In 1/32768 santiBell per 64 tick
    int           mVolDelayEnvelope;   //Delay before sound start. In tick
    int           mVolAttackEnvelope;  //Attack time. Volume rich from zero to peak. In tick
    int           mVolHoldEnvelope;    //Hold time. Volume hold peak level
    int           mVolDecayEnvelope;   //Time for volume linearly ramps toward the sustain level
    int           mVolReleaseEnvelope; //Time
    int           mVolSustainLevel;    //This is the decrease in level, expressed in centibels, to which the Volume Envelope value ramps during the decay phase.
  public:
    SfSynthTrack();

    void setup(const qint16 *samples, int sampleEnd, int sampleLoopStart, int sampleLoopEnd, bool exitLoop, int sampleStep, int volumeInitial,
               int volDelayEnvelope, int volAttackEnvelope, int volHoldEnvelope, int volDecayEnvelope, int volReleaseEnvelope, int volSustainLevel );

    int  sample( bool &stopped );

    void noteOff( quint8 pressure );

    bool noteOn( quint8 pressure );

  private:
    int nextSample();
    int nextSampleIndex();
    int sampleAtIndex() const { return mSamples[mSampleIndex]; }
    int sampleAtNextIndex() const { return mSamples[mSampleIndex + 1]; }

    static int mAttenuator[1024];     //santiBell to K mapping. K in 1/32768
  };

using SfSynthTrackVector = QVector<SfSynthTrack>;

#endif // SFSYNTHTRACK_H
