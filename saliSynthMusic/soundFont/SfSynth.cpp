#include "SfSynth.h"



SfSynth::SfSynth(QObject *parent) : QObject(parent)
  {
  //Build default channels
  for( quint8 i = 0; i < 16; i++ ) {
    SfSynthPresetPtr preset( new SfSynthPreset() );
    //Select programm for it
    preset->programm(i);
    //Change programm to channel
    mChannels[i] = preset;
    //Append programm to map
    mSynthPresetMap.insert( i, preset );
    //Connect programm
    connect( preset.data(), &SfSynthPreset::noteOn, this, &SfSynth::noteOn );
    }
  }





void SfSynth::midi(quint8 cmd, quint8 data0, quint8 data1)
  {
  int channel = cmd & 0xf;
  if( (cmd & 0xf0) == 0xc0 ) {
    //Change programm
    if( mSynthPresetMap.contains(data0) )
      //Programm already present in list, simple change it
      mChannels[channel] = mSynthPresetMap.value( data0 );
    else {
      //Build new preset
      SfSynthPresetPtr preset( new SfSynthPreset() );
      //Select programm for it
      preset->programm(data0);
      //Change programm to channel
      mChannels[channel] = preset;
      //Append programm to map
      mSynthPresetMap.insert( data0, preset );
      //Connect programm
      connect( preset.data(), &SfSynthPreset::noteOn, this, &SfSynth::noteOn );
      }
    }
  else mChannels[channel]->midi( cmd, data0, data1 );
  }
