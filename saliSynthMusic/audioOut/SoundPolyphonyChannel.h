/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#ifndef SOUNDPOLYPHONYCHANNEL_H
#define SOUNDPOLYPHONYCHANNEL_H



class SoundPolyphonyChannel
  {
    int    mPeriod;
    int    mCurrent;
    //double mPeriod;
    int    mSamples[3000];
    int    mId;
    bool   mRun;
    bool   mPlus;
    double mStep;
    double mAngle;
  public:
    SoundPolyphonyChannel();

    //Return next channel sample
    int  sample();

    void setPeriod( int p ) { mPeriod = p; }

    void setChannel( int channel, int note );

    void noteOn( int pressure );

    void noteOff();

    int  id() const { return mId; }

    static int buildId( int channel, int note ) { return (note << 4) | channel; }
  };



#endif // SOUNDPOLYPHONYCHANNEL_H
