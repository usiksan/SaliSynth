/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#include "SoundPolyphonyManager.h"

#include <QDebug>

SoundPolyphonyChannel SoundPolyphonyManager::mSoundPolyphony[POLYPHONY_COUNT];



void SoundPolyphonyManager::noteOff(int channel, int note)
  {
  int id = SoundPolyphonyChannel::buildId( channel, note );
  for( int i = 0; i < POLYPHONY_COUNT; i++ )
    if( mSoundPolyphony[i].id() == id ) {
      mSoundPolyphony[i].noteOff();
      return;
      }
  }



void SoundPolyphonyManager::noteOn(int channel, int note, int pressure)
  {
  int id = SoundPolyphonyChannel::buildId( channel, note );
  int empty = -1;
  for( int i = 0; i < POLYPHONY_COUNT; i++ )
    if( mSoundPolyphony[i].id() == id ) {
      qDebug() << "i" << i << "pressure" << pressure;
      mSoundPolyphony[i].noteOn( pressure );
      return;
      }
    else if( mSoundPolyphony[i].id() < 0 ) empty = i;

  qDebug() << "empty" << empty;
  mSoundPolyphony[empty].setChannel( channel, note );
  mSoundPolyphony[empty].noteOn( pressure );
  }
