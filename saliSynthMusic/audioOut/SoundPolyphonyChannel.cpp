/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#include "SoundPolyphonyChannel.h"

#include <QDebug>

SoundPolyphonyChannel::SoundPolyphonyChannel() :
  mPeriod(0),
  mCurrent(0),
  mId(-1),
  mRun(false)
  {

  }


//One sample from 48000 samples per second
int SoundPolyphonyChannel::sample()
  {
  if( !mRun ) return 0;
//  double sam = sin( mAngle );
//  mAngle += mStep;
//  if( mAngle > M_PI * 2.0 ) mAngle -= M_PI * 2.0;
//  return static_cast<int>( sam * 10000.0 );
  if( mCurrent >= mPeriod ) mCurrent = 0;
  return mSamples[mCurrent++];
  }


static double noteFriq[12] = {
  261.63,
  277.18,
  293.66,
  311.13,
  329.63,
  349.23,
  369.99,
  392.0,
  415.30,
  440.0,
  466.16,
  493.88
};

static double oktavaMult[11] = {
  0.0625, //0  Суб-контр
  0.125,  //1  Контр
  0.25,   //2  Большая
  0.5,    //3  Малая
  1.0,    //4  1-я
  2.0,    //5  2-я
  4.0,    //6  3-я
  8.0,    //7  4-я
  16.0,   //8  5-я
  32.0,   //9  6-я
  64.0    //10 7-я
};

void SoundPolyphonyChannel::setChannel(int channel, int note)
  {
  mId = buildId( channel, note );

  int oktava = note / 12;
  note %= 12;
  double friq = noteFriq[note] * oktavaMult[oktava];
//  qDebug() << "oktava" << oktava << "note" << note << "friq" << friq;
  double samples = 48000.0 / friq;
  int periods = static_cast<int>( 3000.0 / samples );
  mPeriod = static_cast<int>( periods * samples );
  double step = M_PI * 2.0 * periods / mPeriod;
  mStep = step;
  double angle = 0;
//  qDebug() << "samples" << mPeriod << "periods" << periods << "step" << step;
//  qDebug() << "sin" << sin(M_PI);
  for( int i = 0; i < mPeriod; i++ ) {
    mSamples[i] = static_cast<int>( sin(angle) * 2000.0 );
    //qDebug() << i << mSamples[i];
    angle += step;
    }
  //qDebug() << mSamples[mPeriod - 1] << sin(angle);
  }


void SoundPolyphonyChannel::noteOn(int pressure)
  {
  mCurrent = 0;
  if( pressure > 0 ) {
    mRun = true;
    mAngle = 0;
    }
  else {
    mRun = false;
    mId = -1;
    }
//  mRun = pressure > 0;
  }


void SoundPolyphonyChannel::noteOff()
  {
  mRun = false;
  mId = -1;
  }
