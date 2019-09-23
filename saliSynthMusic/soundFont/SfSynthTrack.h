#ifndef SFSYNTHTRACK_H
#define SFSYNTHTRACK_H

#include <QVector>
#include <stdlib.h>

#define dB 1.12201845430196

//Synth one track from one instrument
class SfSynthTrack
  {
    const qint16 *mSamples;     //Samples vector
    int           mSampleStart; //Sample start index
    int           mSampleEnd;
    int           mSampleLoopStart;
    int           mSampleLoopEnd;
    bool          mExitLoop;        //Walk through end loop, else looping
    bool          mRun;             //If true then generating samples

    //Oscillator
    int           mSampleIndex;     //Current sample index
    int           mSampleSubIndex;  //Fractional part of sample index in 1/65536
    int           mSampleStep;      //Step of sample to provide a given sample rate. In 1/65536

    //Amplitude envelope modulator
    enum {
      vpStop,
      vpDelay,
      vpAttack,
      vpHold,
      vpDecay,
      vpRelease,
      }           mVolumePhase;
    int           mVolumeTick;
    int           mVolDelayEnvelope;  //Delay before sound start. In tick
    int           mVolAttackEnvelope; //Attack time. Volume rich from zero to peak
    int           mVolHoldEnvelope;   //Hold time. Volume hold peak level
    int           mVolDecayEnvelope;  //Time for volume linearly ramps toward the sustain level
    int           mVolReleaseEnvelope; //Time
    int           mVolSustainLevel;
  public:
    SfSynthTrack();

    int  sample( bool &stopped );

    void noteOff( quint8 pressure );

    bool noteOn( quint8 pressure );

  private:
    int nextSample();
    int nextSampleIndex();
    int sampleAtIndex() const { return mSamples[mSampleIndex]; }
    int sampleAtNextIndex() const { return mSamples[mSampleIndex + 1]; }
  };

using SfSynthTrackVector = QVector<SfSynthTrack>;

#endif // SFSYNTHTRACK_H
