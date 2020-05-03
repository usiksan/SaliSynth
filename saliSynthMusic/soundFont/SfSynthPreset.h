/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Single preset for synth. Preset is single or set of instruments acts as the whole.
   Preset sounds when you press a key (midi event from keyboard or midi file or accompaniment)
*/
#ifndef SFSYNTHPRESET_H
#define SFSYNTHPRESET_H

#include "SfSynthNote.h"
#include "SoundFont.h"

#include <QObject>

class SfSynth;

class SfSynthPreset
  {
    QString      mVoiceName;    //! Voice name
    SfSynthNote  mNotes[128];   //! Builded notes for preset
    SoundFontPtr mSoundFontPtr; //! Sound font synth base on
    int          mVoiceId;      //! Id of current voice
  public:
    explicit SfSynthPreset();

    QString      voiceName() const { return mVoiceName; }

    int          voiceId() const { return mVoiceId; }

    SoundFontPtr soundFontPtr() const { return mSoundFontPtr; }

    //Action on midi event
    void         midi( SfSynth *synth, quint8 cmd, quint8 data0, quint8 data1 );

    void         build( int voiceId, const QString voiceName, SoundFontPtr soundFont, int preset );

    void         clone(const SfSynthPreset *src );

  private:
    //!
    //! \brief setVolume Setup master volume for all notes of preset
    //! \param volume    Volume value which need to be installed
    //!
    void         setVolume( int volume );
  };


#endif // SFSYNTHPRESET_H
