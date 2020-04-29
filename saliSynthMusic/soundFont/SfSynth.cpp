#include "SfSynth.h"
#include "SvQml/SvDir.h"
#include "SvQml/SvQmlUtils.h"

#include <QDebug>


static int voiceId( int bankMsb, int bankLsb, int midiProgram )
  {
  return ((bankMsb & 0x7f) << 14) | ((bankLsb & 0x7f) << 7) | (midiProgram & 0x7f);
  }

//static int voiceBankMsb


SfSynth::SfSynth(QObject *parent) :
  QObject(parent),
  mVoiceList(nullptr),
  mChannelList(nullptr),
  mMidiConnected(false)
  {
  }



void SfSynth::setVoiceList(SvQmlJsonModel *md)
  {
  mVoiceList = md;
  }



void SfSynth::setChannelList(SvQmlJsonModel *md)
  {
  mChannelList = md;

  if( mChannelList->count() == 0 ) {
    //Create channel list
    for( int i = 0; i < 16; i++ ) {
      mChannelList->addRecord();
      mChannelList->setInt( i, QString("channelBankMsb"), 0 );
      mChannelList->setInt( i, QString("channelBankLsb"), 0 );
      mChannelList->setInt( i, QString("channelProgram"), i );
      }
    }

  if( mVoiceList ) {
    for( int i = 0; i < 16; i++ ) {
      int bankMsb = mChannelList->asInt( i, QString("channelBankMsb") );
      int bankLsb = mChannelList->asInt( i, QString("channelBankLsb") );
      int program = mChannelList->asInt( i, QString("channelProgram") );
      channelSetVoiceId( i, voiceId( bankMsb, bankLsb, program ) );
      }
    }
  }










QStringList SfSynth::presetList(int voiceRow)
  {
  SoundFontPtr soundFontPtr = soundFont( mVoiceList->asString( voiceRow, QString("voiceSoundFontFile")) );
  if( soundFontPtr.isNull() )
    //No sound font assigned to programm
    return QStringList{};
  return soundFontPtr->presetList();
  }







QString SfSynth::soundFontPath() const
  {
  return SvQmlUtils::getHomePath() + "saliSynthMusic/soundFonts/";
  }




bool SfSynth::containsVoice(int bankMsb, int bankLsb, int midiProgram)
  {
  return voiceRow( bankMsb, bankLsb, midiProgram ) >= 0;
  }



int SfSynth::voiceRow(int bankMsb, int bankLsb, int midiProgram)
  {
  return voiceRowById( voiceId( bankMsb, bankLsb, midiProgram ) );
  }




int SfSynth::voiceRowById(int voiceId)
  {
  for( int i = 0; i < mVoiceList->count(); i++ )
    if( mVoiceList->asInt( i, "voiceId") == voiceId )
      return i;
  return -1;
  }




void SfSynth::emitNoteOn(SfSynthNote *note)
  {
  emit noteOn( note );
  }





void SfSynth::midiSlot(quint8 cmd, quint8 data0, quint8 data1)
  {
  if( mVoiceList == nullptr ) return;
  int channel = cmd & 0xf;
  if( (cmd & 0x70) == 0x40 )
    //Change programm
    setProgramm( channel, data0 );
  else if( (cmd & 0x70) == 0x30 && (data0 == 0 || data0 == 0x20) ) {
    if( data0 == 0 )
      //Bank MSB
      mChannelList->setInt( channel, QString("channelBankMsb"), data1 & 0x7f );
    else
      //Bank LSB
      mChannelList->setInt( channel, QString("channelBankLsb"), data1 & 0x7f );
    }
  else
    //Common midi
    mChannels[channel].midi( this, cmd, data0, data1 );
  }




void SfSynth::setProgramm(int channel, int programm)
  {
  channel &= 0xf;
  programm &= 0x7f;
  int bankMsb = mChannelList->asInt( channel, QString("channelBankMsb") );
  int bankLsb = mChannelList->asInt( channel, QString("channelBankLsb") );
  channelSetVoiceId( channel, voiceId( bankMsb, bankLsb, programm ) );
  }





void SfSynth::midiConnection(bool on)
  {
  mMidiConnected = on;
  emit midiConnectedChanged();
  }





void SfSynth::applySoundFont(int voiceRow, const QString soundFont, int preset)
  {
  mVoiceList->setString( voiceRow, QString("voiceSoundFontFile"), soundFont );
  applyPreset( voiceRow, preset );
  }




void SfSynth::applyPreset(int voiceRow, int preset)
  {
  SoundFontPtr soundFontPtr = soundFont( mVoiceList->asString( voiceRow, QString("voiceSoundFontFile")) );
  if( !soundFontPtr.isNull() ) {
    //Assign textual name
    mVoiceList->setString( voiceRow, QString("voiceSoundFontPresetName"), soundFontPtr->presetName(preset) );
    //..and preset index
    mVoiceList->setInt( voiceRow, QString("voiceSoundFontPreset"), preset );
    //By default visual voice name is a same as preset if it previously not assigned
    if( mVoiceList->asString( voiceRow, QString("voiceName")).isEmpty() )
      mVoiceList->setString( voiceRow, QString("voiceName"), soundFontPtr->presetName(preset) );

    //Because font and/or preset changed we must rebuild all appropriate presets
    int id = mVoiceList->asInt( voiceRow, QString("voiceId") );
    if( mPresetCache.contains(id) )
      mPresetCache.object(id)->build( id, soundFontPtr, preset );
    for( int i = 0; i < 16; i++ )
      if( mChannels[i].id() == id )
        channelSetVoiceRow( i, voiceRow );
    }
  }



void SfSynth::channelSetVoiceId(int channel, int voiceId)
  {
  channelSetVoiceRow( channel, voiceRowById(voiceId) );
  }




void SfSynth::channelSetVoiceRow(int channel, int voiceRow)
  {
  channel &= 0xf;
  if( mVoiceList != nullptr && voiceRow >= 0 && voiceRow < mVoiceList->count() ) {
    int voiceId = mVoiceList->asInt( voiceRow, QString("voiceId") );
    //Check if there voice in cache
    if( !mPresetCache.contains(voiceId) ) {
      //No voice in cache, create
      SfSynthPreset *presetPtr = new SfSynthPreset();
      presetPtr->build( voiceId, soundFont(mVoiceList->asString( voiceRow, QString("voiceSoundFontFile"))), mVoiceList->asInt( voiceRow, QString("voiceSoundFontPreset")) );
      //... and append to the cache
      mPresetCache.insert( voiceId, presetPtr, 5 );
      }

    //At this moment preset exactly in the cache
    mChannels[channel].clone( mPresetCache.object(voiceId) );

    //Update visual channel list
    mChannelList->setString( channel, QString("channelVoiceName"), mChannels[channel].name() );
    mChannelList->setInt( channel, QString("channelBankMsb"), (voiceId >> 14) & 0x7f );
    mChannelList->setInt( channel, QString("channelBankLsb"), (voiceId >> 7) & 0x7f );
    mChannelList->setInt( channel, QString("channelProgram"), (voiceId) & 0x7f );
    mChannelList->setInt( channel, QString("channelVoiceId"), voiceId );
    }
  }




void SfSynth::voiceAdd()
  {
  int row = mVoiceList->count();
  mVoiceList->addRecord();
  mVoiceList->setInt( row, "voiceBankMsb", 128 );
  mVoiceList->setInt( row, "voiceBankLsb", 128 );
  mVoiceList->setInt( row, "voiceProgram", 128 );
  mVoiceList->setInt( row, "voiceId", -1 );
  }





void SfSynth::voiceDuplicate(int voiceRow)
  {
  int row = mVoiceList->count();
  mVoiceList->addRecord();
  mVoiceList->setInt( row, "voiceBankMsb", mVoiceList->asInt(voiceRow, "voiceBankMsb") );
  mVoiceList->setInt( row, "voiceBankLsb", mVoiceList->asInt(voiceRow, "voiceBankLsb") );
  mVoiceList->setInt( row, "voiceProgram", 128 );
  mVoiceList->setInt( row, "voiceId", -1 );

  mVoiceList->setString( row, QString("voiceSoundFontFile"), mVoiceList->asString( voiceRow, QString("voiceSoundFontFile")) );
  mVoiceList->setString( row, QString("voiceSoundFontPresetName"), mVoiceList->asString( voiceRow, QString("voiceSoundFontPresetName")) );
  mVoiceList->setInt( row, QString("voiceSoundFontPreset"), mVoiceList->asInt( voiceRow, QString("voiceSoundFontPreset")) );
  mVoiceList->setString( row, QString("voiceName"), mVoiceList->asString( voiceRow, QString("voiceName")) );
  }



bool SfSynth::voiceSettings(int voiceRow, int bankMsb, int bankLsb, int prog)
  {
  if( (bankMsb & 0xffffff80) == 0 && (bankLsb & 0xffffff80) == 0 && (prog & 0xffffff80) == 0  ) {
    //Valid voice id
    //Check if voice already defined
    if( !containsVoice( bankMsb, bankLsb, prog ) ) {
      //No voice with thease id's
      int id = voiceId( bankMsb, bankLsb, prog );

      mVoiceList->setInt( voiceRow, "voiceBankMsb", bankMsb );
      mVoiceList->setInt( voiceRow, "voiceBankLsb", bankLsb );
      mVoiceList->setInt( voiceRow, "voiceProgram", prog );
      mVoiceList->setInt( voiceRow, "voiceId", id );
      }
    else return false;
    }
  else {
    mVoiceList->setInt( voiceRow, "voiceBankMsb", bankMsb );
    mVoiceList->setInt( voiceRow, "voiceBankLsb", bankLsb );
    mVoiceList->setInt( voiceRow, "voiceProgram", prog );
    mVoiceList->setInt( voiceRow, "voiceId", -1 );
    }
  return true;
  }




SoundFontPtr SfSynth::soundFont(const QString fontName)
  {
  if( mSoundFontMap.contains(fontName) ) {
    if( !mSoundFontMap.value(fontName).isNull() )
      return mSoundFontMap.value(fontName).toStrongRef();
    mSoundFontMap.remove(fontName);
    }
  //Sound font not yet loaded
  //We creating sound font
  SoundFontPtr soundFontPtr( new SoundFont );
  //...load it
  soundFontPtr->read( soundFontPath() + fontName + ".sf2" );
  //...and append to map
  mSoundFontMap.insert( fontName, soundFontPtr );
  return soundFontPtr;
  }

