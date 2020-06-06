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
#ifndef SFSYNTHNOTE_H
#define SFSYNTHNOTE_H

#include "SfSynthTrack.h"
#include "SoundFont.h"

class SfSynthVoice;

//Note consist of one or more tracks
//Preset or program is 128 or less notes
class SfSynthNote
  {
  protected:
    SfSynthTrackVector mTracks;  //! Tracks of which consist a note
    int                mNote;    //! Note index
    bool               mStopped; //! Flag, true when no note sounds
  public:
    SfSynthNote();

    //Set note index
    void setNote( int note ) { mNote = note; }

    //Clear builded note to its default cleared nondefined state
    void clear();

    //Get next sound sample for this note
    int  sample();

    //Stop flag state
    bool isStopped() const { return mStopped; }

    void noteOff( quint8 pressure );

    bool noteOn( quint8 pressure );

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
    bool addTrack( quint16 *generator, const SfSample &sam, qint16 *samples, int endSample, int startLoop, int endLoop,
                   int delayVolEnv, int attackVolEnv, int holdVolEnv, int decayVolEnv, int releaseVolEnv );

    //!
    //! \brief setVolume Setup master volume for all tracks of note
    //! \param volume    Volume value which need to be installed
    //!
    void setVolume( int volume );

    int  getVolume() const { return mTracks.count() ? mTracks.first().mMasterVolume : 127; }
  };

#endif // SFSYNTHNOTE_H
