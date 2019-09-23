#ifndef SFSYNTHTRACK_H
#define SFSYNTHTRACK_H

#include <QVector>
#include <stdlib.h>

#define dB 1.12201845430196

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
    int           mSampleRate;      //Sample rate in Hz
    int           mSampleIndex;     //Current sample index
    int           mSampleSubIndex;  //Fractional part of sample index in 1/65536
    int           mSampleStep;      //Step of sample to provide a given sample rate. In 1/65536

    //Amplitude envelope modulator
    //int
    int           mVolDelayEnvelope;  //Delay before sound start. In tick
    int           mVolAttackEnvelope; //Attack time. Volume rich from zero to peak
    int           mVolHoldEnvelope;   //Hold time. Volume hold peak level
    int           mVolDecayEnvelope;  //Time for volume linearly ramps toward the sustain level
    int           mVolReleaseEnvelope; //Time
    int           mVolSustainLevel;
  public:
    SfSynthTrack();

  private:
    int nextSample();
    int nextSampleIndex();
    int sampleAtIndex() const { return mSamples[mSampleIndex]; }
    int sampleAtNextIndex() const { return mSamples[mSampleIndex + 1]; }
  };

#endif // SFSYNTHTRACK_H
