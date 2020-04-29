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
    QString      mName;         //! Preset name
    SfSynthNote  mNotes[128];   //! Builded notes for preset
    SoundFontPtr mSoundFontPtr; //! Sound font synth base on
    int          mId;           //! Id of current preset
  public:
    explicit SfSynthPreset();

    QString      name() const { return mName; }

    int          id() const { return mId; }

    SoundFontPtr soundFontPtr() const { return mSoundFontPtr; }

    //Action on midi event
    void         midi( SfSynth *synth, quint8 cmd, quint8 data0, quint8 data1 );

    void         build( int id, SoundFontPtr soundFont, int preset );

    void         clone(const SfSynthPreset *src );

  };


#endif // SFSYNTHPRESET_H
