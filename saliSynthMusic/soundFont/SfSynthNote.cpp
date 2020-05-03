/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Single note for synth. Note is single or set of sound played simultaneously.
   For this, note consist of one or more tracks each of that is separate sound
   generator.
   Note sounds when midi events.
*/
#include "SfSynthNote.h"
#include <math.h>
#include <QDebug>

SfSynthNote::SfSynthNote() :
  mStopped(true)
  {

  }

//Clear builded note to its default cleared nondefined state
void SfSynthNote::clear()
  {
  //Stop note sounds
  mStopped = true;
  //Remove all note tracks
  mTracks.clear();
  }




//Get next sound sample for this note
int SfSynthNote::sample()
  {
  if( !mStopped && mTracks.count() ) {

    //Calculate sum of all tracks
    int sampleSum = 0;

    //Stopped flag set to false if any track is not completed
    bool stopped = true;

    //Generate next sample for each track
    for( SfSynthTrack &track : mTracks )
      sampleSum += track.sample( stopped );

    //Update stop flag
    mStopped = stopped;

    //Return tracks sample sum
    return sampleSum;
    }

  //Note stopped or no track, so we return 0
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




//!
//! \brief addTrack         Build track for note and add it to the note track vector
//! \param generator        Current generator state
//! \param sam              Sample descriptor for this track
//! \param samples          Samples array from begin of which by other params we select fragment
//! \param endSample        Index where samples fragment is ended
//! \param startLoop        Index where loop starts in samples fragment
//! \param endLoop          Index where loop ends in samples fragment
//! \param delayVolEnv      Delay after key on and before sound
//! \param attackVolEnv     Attack phase time
//! \param holdVolEnv       Hold phase time
//! \param decayVolEnv      Decay phase time
//! \param releaseVolEnv    Release phase time
//! \return                 true if track builded and added successfuly
//!
bool SfSynthNote::addTrack( quint16 *generator, const SfSample &sam, qint16 *samples, int endSample, int startLoop, int endLoop,
                            int delayVolEnv, int attackVolEnv, int holdVolEnv, int decayVolEnv, int releaseVolEnv )
  {
//  qDebug() << "generator:";
//  for( int i = 0; i < 61; i++ )
//    qDebug() << i << generator[i];

  //Original note
  int originalNote = generator[sfpiOverridingRootKey];
  if( originalNote < 0 || originalNote > 127 )
    originalNote = sam.byOriginalPitch;
  if( originalNote < 0 || originalNote > 127 )
    originalNote = 60;

  //Synth step calculation
  //Base friquency of current note
  double friq = noteFriq[mNote % 12] * oktavaMult[mNote / 12];
  //Friquency of original note
  double originalFriq  = noteFriq[originalNote % 12] * oktavaMult[originalNote / 12];
  //Step for original note
  double originalStep = static_cast<double>(sam.dwSampleRate) / 48000.0;
  //Step for current note
  //orig = 1000Hz, friq = 500Hz
  //if step for orig = 1, then for friq it must be = 0.5
  //so step
  double step = originalStep * friq / originalFriq;

  //Timings calculation
  //Because we shift this note by original, then timings must be corrected
  int timeMulter = 64.0 * originalFriq / friq;
  if( releaseVolEnv < 1000 )
    releaseVolEnv = releaseVolEnv * timeMulter >> 6;


  SfSynthTrack track;
  track.setup( samples, endSample, startLoop, endLoop, (generator[sfpiSampleModes] & 1) == 0,
               static_cast<int>( step * 65536.0 ), generator[sfpiInitialAttenuation],
               delayVolEnv, attackVolEnv, holdVolEnv, decayVolEnv, releaseVolEnv,
               generator[sfpiSustainVolEnv], generator[sfpiVelRange]
               );
  //if( mTracks.count() == 0 )
  mTracks.append( track );
  //qDebug() << "note" << mNote << mTracks.count();
  return true;
  }




//!
//! \brief setVolume Setup master volume for all tracks of note
//! \param volume    Volume value which need to be installed
//!
void SfSynthNote::setVolume(int volume)
  {
  for( auto &t : mTracks )
    t.mMasterVolume = volume;
  }



