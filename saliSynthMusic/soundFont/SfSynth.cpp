#include "SfSynth.h"
#include "SvQml/SvDir.h"
#include "SvQml/SvQmlUtils.h"

#include <QDebug>


SfSynth::SfSynth(QObject *parent) :
  QObject(parent),
  mModel(nullptr),
  mMidiConnected(false)
  {
  }




void SfSynth::setModel(SvQmlJsonModel *md)
  {
  mModel = md;

  //Test if model empty then initialize model
  if( mModel->count() == 0 ) {
    //Initialize model
    for( int i = 0; i < 128; i++ )
      addModelRecord( i, QString("Instrument %1").arg(i) );
    }

  //Fill programms
  for( int i = 0; i < 128; i++ ) {
    //Create programm
    SfSynthPresetPtr preset( new SfSynthPreset() );
    mProgramms[i] = preset;
    //Connect programm
    connect( preset.data(), &SfSynthPreset::noteOn, this, &SfSynth::noteOn );

    QString soundFont = mModel->asString( i, "soundFontFile" );
    if( !soundFont.isEmpty() )
      //Sound font assigned for this programm slot
      applySoundFont( i, soundFont, mModel->asInt( i, "preset" ) );
    }

  //Build default channels
  for( quint8 i = 0; i < 16; i++ )
    //Change programm to channel
    mChannels[i] = mProgramms[i];
  }






QStringList SfSynth::presetList(int programm)
  {
  SoundFontPtr soundFontPtr = mProgramms[ programm & 0x7f ]->soundFontPtr();
  if( soundFontPtr.isNull() )
    //No sound font assigned to programm
    return QStringList{};
  return soundFontPtr->presetList();
  }







QString SfSynth::soundFontPath() const
  {
  return SvQmlUtils::getHomePath() + "saliSynthMusic/soundFonts/";
  }





void SfSynth::midi(quint8 cmd, quint8 data0, quint8 data1)
  {
  if( mModel == nullptr ) return;
  int channel = cmd & 0xf;
  if( (cmd & 0x70) == 0x40 )
    //Change programm
    setProgramm( channel, data0 );
  else
    //Common midi
    mChannels[channel]->midi( cmd, data0, data1 );
  }




void SfSynth::setProgramm(int channel, int programm)
  {
  channel &= 0xf;
  programm &= 0x7f;
  mChannels[channel] = mProgramms[programm];
  }





void SfSynth::midiConnection(bool on)
  {
  mMidiConnected = on;
  emit midiConnectedChanged();
  }





void SfSynth::applySoundFont(int programm, const QString soundFont, int preset)
  {
  mModel->setString( programm, "soundFontFile", soundFont );
  mModel->setInt( programm, "preset", preset );
  qDebug() << "sound font" << soundFont;
  //Check if it new file
  if( !mSoundFontMap.contains( soundFont ) || mSoundFontMap.value(soundFont).isNull() ) {
    //Sound font not yet loaded
    //We creating sound font
    SoundFontPtr soundFontPtr( new SoundFont );
    //...load it
    soundFontPtr->read( soundFontPath() + soundFont + ".sf2" );
    //...and append to map
    mSoundFontMap.insert( soundFont, soundFontPtr );

    mProgramms[programm & 0x7f]->build( soundFontPtr, preset );
    }
  else
    mProgramms[programm & 0x7f]->build( mSoundFontMap.value(soundFont).toStrongRef(), preset );
  mModel->setString( programm, "presetName", mSoundFontMap.value(soundFont).toStrongRef()->presetName(preset) );
  }




void SfSynth::applyPreset(int programm, int preset)
  {
  programm &= 0x7f;
  SoundFontPtr soundFontPtr = mProgramms[ programm & 0x7f ]->soundFontPtr();
  if( !soundFontPtr.isNull() ) {
    //No sound font assigned to programm
    mProgramms[programm]->build(soundFontPtr, preset );
    //Assign textual name
    mModel->setString( programm, "presetName", soundFontPtr->presetName(preset) );
    //..and preset index
    mModel->setInt( programm, "preset", preset );
    }
  }




void SfSynth::addModelRecord(int index, const QString title)
  {
  mModel->addRecord();
  mModel->setString( index, "instrumentTitle", title );
  mModel->setInt( index, "programm", index );
  }

