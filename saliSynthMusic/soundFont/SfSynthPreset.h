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


class SfSynthPreset : public QObject
  {
    Q_OBJECT

    SfSynthNote  mNotes[128];   //Builded notes for preset
    SoundFontPtr mSoundFontPtr; //Sound font synth base on
  public:
    explicit SfSynthPreset(QObject *parent = nullptr);


  signals:
    //Append note to audio output to synth it
    void noteOn( SfSynthNote *note );

  public slots:
    //Action on midi event
    void midi( quint8 cmd, quint8 data0, quint8 data1 );

    //Change current programm of preset
    void programm( quint8 prg );

  private:

    void build( SoundFontPtr soundFont, int preset );
  };

using SfSynthPresetPtr = QSharedPointer<SfSynthPreset>;

#endif // SFSYNTHPRESET_H
