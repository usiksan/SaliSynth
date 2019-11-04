#include "SfSynth.h"



SfSynth::SfSynth(QObject *parent) :
  QObject(parent),
  mModel(nullptr)
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
//    addModelRecord( 0, tr("Grand piano") );
//    addModelRecord( 1, tr("Bright piano") );
//    addModelRecord( 2, tr("E Grand piano") );
//    addModelRecord( 3, tr("Honky-tonk") );
//    addModelRecord( 4, tr("E Piano 1") );
//    addModelRecord( 5, tr("E Piano 2") );
//    addModelRecord( 6, tr("Harpsichord") );
//    addModelRecord( 7, tr("Clavi") );

//    addModelRecord( 8, tr("Celesta") );
//    addModelRecord( 9, tr("Glockenspiel") );
//    addModelRecord( 10, tr("Music box") );
//    addModelRecord( 11, tr("Vibraphone") );
    }

  //Fill programms
  SfSynthPresetPtr defPreset( new SfSynthPreset() );
  for( int i = 0; i < 128; i++ ) {
    QString soundFont = mModel->asString( i, "soundFontFile" );
    if( soundFont.isEmpty() ) {
      //No sound font assigned for this programm slot
      mProgramms[i] = defPreset;
      }
    else {
      //Sound font assigned for this programm slot
      if( !mSoundFontMap.contains( soundFont ) ) {
        //Sound font not yet loaded
        //We creating sound font
        SoundFontPtr soundFontPtr( new SoundFont );
        //...load it
        soundFontPtr->read( soundFont );
        //...and append to map
        mSoundFontMap.insert( soundFont, soundFontPtr );
        }

      SfSynthPresetPtr preset( new SfSynthPreset() );
      preset->build( mSoundFontMap.value(soundFont), mModel->asInt( i, "preset" ) );
      //Connect programm
      connect( preset.data(), &SfSynthPreset::noteOn, this, &SfSynth::noteOn );
      }
    }

  //Build default channels
  for( quint8 i = 0; i < 16; i++ )
    //Change programm to channel
    mChannels[i] = mProgramms[i];
  }



QStringList SfSynth::presetList(const QString soundFont )
  {
  if( mSoundFontMap.contains( soundFont ) )
    return mSoundFontMap.value( soundFont )->presetList();

  //No sound font with this name
  return QStringList{};
  }





void SfSynth::midi(quint8 cmd, quint8 data0, quint8 data1)
  {
  if( mModel == nullptr ) return;
  int channel = cmd & 0xf;
  if( (cmd & 0xf0) == 0xc0 )
    //Change programm
    mChannels[channel] = mProgramms[data0 & 0x7f];
  else
    //Common midi
    mChannels[channel]->midi( cmd, data0, data1 );
  }





void SfSynth::applySoundFont(int programm, const QString soundFont, int preset)
  {
  //Check if it new file
  if( !mSoundFontMap.contains( soundFont ) ) {
    //Sound font not yet loaded
    //We creating sound font
    SoundFontPtr soundFontPtr( new SoundFont );
    //...load it
    soundFontPtr->read( soundFont );
    //...and append to map
    mSoundFontMap.insert( soundFont, soundFontPtr );
    }

  mProgramms[programm & 0x7f]->build( mSoundFontMap.value(soundFont), preset );
  }




void SfSynth::addModelRecord(int index, const QString title)
  {
  mModel->addRecord();
  mModel->setString( index, "title", title );
  mModel->setInt( index, "programm", index );
  }
