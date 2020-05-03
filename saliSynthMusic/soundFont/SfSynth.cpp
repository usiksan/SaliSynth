#include "SfSynth.h"
#include "SvQml/SvDir.h"
#include "SvQml/SvQmlUtils.h"

#include <QDebug>


static int voiceId( int bankMsb, int bankLsb, int midiProgram )
  {
  return ((bankMsb & 0x7f) << 14) | ((bankLsb & 0x7f) << 7) | (midiProgram & 0x7f);
  }



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

  if( mChannelList ) {
    //On base current channel list restores channels
    for( quint8 i = 0; i < 16; i++ )
      channelSetVoiceId( i, mChannelList->asInt( i, QStringLiteral("channelVoiceId") ) );
    }
  }



void SfSynth::setChannelList(SvQmlJsonModel *md)
  {
  mChannelList = md;

  if( mChannelList->count() == 0 ) {
    //Create channel list
    for( int i = 0; i < 16; i++ ) {
      mChannelList->addRecord();
      mChannelList->setInt( i, QStringLiteral("channelVoiceId"), i );
      }
    }

  if( mVoiceList ) {
    //On base current channel list restores channels
    for( quint8 i = 0; i < 16; i++ )
      channelSetVoiceId( i, mChannelList->asInt( i, QStringLiteral("channelVoiceId") ) );
    }
  }










QStringList SfSynth::presetList(int voiceRow)
  {
  SoundFontPtr soundFontPtr = soundFont( mVoiceList->asString( voiceRow, QStringLiteral("voiceSoundFontFile")) );
  if( soundFontPtr.isNull() )
    //No sound font assigned to programm
    return QStringList{};
  return soundFontPtr->presetList();
  }







QString SfSynth::soundFontPath() const
  {
  return SvQmlUtils::getHomePath() + "saliSynthMusic/soundFonts/";
  }

QString SfSynth::midiPath() const
  {
  return SvQmlUtils::getHomePath() + "midi/";
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
    if( mVoiceList->asInt( i, QStringLiteral("voiceId")) == voiceId )
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
      mChannelList->setInt( channel, QStringLiteral("channelBankMsb"), data1 & 0x7f );
    else
      //Bank LSB
      mChannelList->setInt( channel, QStringLiteral("channelBankLsb"), data1 & 0x7f );
    }
  else
    //Common midi
    mChannels[channel].midi( this, cmd, data0, data1 );
  }




void SfSynth::setProgramm(int channel, int programm)
  {
  channel &= 0xf;
  programm &= 0x7f;
  int bankMsb = mChannelList->asInt( channel, QStringLiteral("channelBankMsb") );
  int bankLsb = mChannelList->asInt( channel, QStringLiteral("channelBankLsb") );
  channelSetVoiceId( channel, voiceId( bankMsb, bankLsb, programm ) );
  }





void SfSynth::midiConnection(bool on)
  {
  mMidiConnected = on;
  emit midiConnectedChanged();
  }





void SfSynth::applySoundFont(int voiceRow, const QString soundFont, int preset)
  {
  mVoiceList->setString( voiceRow, QStringLiteral("voiceSoundFontFile"), soundFont );
  applyPreset( voiceRow, preset );
  }




void SfSynth::applyPreset(int voiceRow, int preset)
  {
  SoundFontPtr soundFontPtr = soundFont( mVoiceList->asString( voiceRow, QStringLiteral("voiceSoundFontFile")) );
  if( !soundFontPtr.isNull() )
    applyPresetFromFont( voiceRow, soundFontPtr, preset );
  }



void SfSynth::channelSetVoiceId(quint8 channel, int voiceId)
  {
  channelSetVoiceRow( channel, voiceRowById(voiceId) );
  }




void SfSynth::channelSetVoiceRow(int channel, int voiceRow)
  {
  channel &= 0xf;
  if( mVoiceList != nullptr && voiceRow >= 0 && voiceRow < mVoiceList->count() ) {
    int voiceId = mVoiceList->asInt( voiceRow, QStringLiteral("voiceId") );
    //Check if there voice in cache
    if( !mPresetCache.contains(voiceId) ) {
      //No voice in cache, create
      SfSynthPreset *presetPtr = new SfSynthPreset();
      presetPtr->build( voiceId, mVoiceList->asString( voiceRow, QStringLiteral("voiceName")),
                        soundFont(mVoiceList->asString( voiceRow, QStringLiteral("voiceSoundFontFile"))),
                        mVoiceList->asInt( voiceRow, QStringLiteral("voiceSoundFontPreset")) );
      //... and append to the cache
      mPresetCache.insert( voiceId, presetPtr, 5 );
      }

    //At this moment preset exactly in the cache
    mChannels[channel].clone( mPresetCache.object(voiceId) );
    if( channel == 0 ) emit rightMainVoiceChanged();
    else if( channel == 1 ) emit leftVoiceChanged();
    else if( channel == 2 ) emit rightSlaveVoiceChanged();

    //Update visual channel list
    mChannelList->setInt( channel, QStringLiteral("channelVoiceId"), voiceId );
    }
  }




void SfSynth::voiceAdd()
  {
  int row = mVoiceList->count();
  mVoiceList->addRecord();
  mVoiceList->setInt( row, QStringLiteral("voiceBankMsb"), 128 );
  mVoiceList->setInt( row, QStringLiteral("voiceBankLsb"), 128 );
  mVoiceList->setInt( row, QStringLiteral("voiceProgram"), 128 );
  mVoiceList->setInt( row, QStringLiteral("voiceId"), -1 );
  }





void SfSynth::voiceDuplicate(int voiceRow)
  {
  int row = mVoiceList->count();
  mVoiceList->addRecord();
  mVoiceList->setInt( row, QStringLiteral("voiceBankMsb"), mVoiceList->asInt(voiceRow, QStringLiteral("voiceBankMsb")) );
  mVoiceList->setInt( row, QStringLiteral("voiceBankLsb"), mVoiceList->asInt(voiceRow, QStringLiteral("voiceBankLsb")) );
  mVoiceList->setInt( row, QStringLiteral("voiceProgram"), 128 );
  mVoiceList->setInt( row, QStringLiteral("voiceId"), -1 );

  mVoiceList->setString( row, QStringLiteral("voiceSoundFontFile"), mVoiceList->asString( voiceRow, QStringLiteral("voiceSoundFontFile")) );
  mVoiceList->setString( row, QStringLiteral("voiceSoundFontPresetName"), mVoiceList->asString( voiceRow, QStringLiteral("voiceSoundFontPresetName")) );
  mVoiceList->setInt( row, QStringLiteral("voiceSoundFontPreset"), mVoiceList->asInt( voiceRow, QStringLiteral("voiceSoundFontPreset")) );
  mVoiceList->setString( row, QStringLiteral("voiceIconName"), mVoiceList->asString( voiceRow, QStringLiteral("voiceIconName")) );
  mVoiceList->setString( row, QStringLiteral("voiceName"), mVoiceList->asString( voiceRow, QStringLiteral("voiceName")) );
  }




void SfSynth::voiceFontExtractAll(int voiceRow)
  {
  int voiceId = mVoiceList->asInt( voiceRow, "voiceId" );
  if( voiceId < 0 ) return;
  QString soundFontName = mVoiceList->asString( voiceRow, QStringLiteral("voiceSoundFontFile") );
  QString iconName = mVoiceList->asString( voiceRow, QStringLiteral("voiceIconName") );
  SoundFontPtr soundFontPtr = soundFont( soundFontName );
  if( !soundFontPtr.isNull() ) {
    int presetCount = soundFontPtr->presetCount() - 1;
    for( int i = mVoiceList->asInt( voiceRow, QStringLiteral("voiceSoundFontPreset") ) + 1; i < presetCount; i++ ) {
      //Append row
      voiceAdd();
      //Take next id and test if voice not exist
      for( voiceId++; voiceRowById(voiceId) >= 0; voiceId++ );
      //No available slots
      if( voiceId >= 127 * 127 * 127 ) return;
      int row = mVoiceList->count() - 1;
      //Fill info
      mVoiceList->setString( row, QStringLiteral("voiceSoundFontFile"), soundFontName );
      mVoiceList->setString( row, QStringLiteral("voiceIconName"), iconName );

      mVoiceList->setInt( row, QStringLiteral("voiceBankMsb"), (voiceId >> 14) & 0x7f );
      mVoiceList->setInt( row, QStringLiteral("voiceBankLsb"), (voiceId >> 7) & 0x7f );
      mVoiceList->setInt( row, QStringLiteral("voiceProgram"), (voiceId) & 0x7f );
      mVoiceList->setInt( row, QStringLiteral("voiceId"), voiceId );

      //At end we apply preset
      applyPresetFromFont( row, soundFontPtr, i );
      }
    }

  }



bool SfSynth::voiceSettings(int voiceRow, int bankMsb, int bankLsb, int prog)
  {
  if( (bankMsb & 0xffffff80) == 0 && (bankLsb & 0xffffff80) == 0 && (prog & 0xffffff80) == 0  ) {
    //Valid voice id
    //Check if voice already defined
    if( !containsVoice( bankMsb, bankLsb, prog ) ) {
      //No voice with thease id's
      int id = voiceId( bankMsb, bankLsb, prog );

      mVoiceList->setInt( voiceRow, QStringLiteral("voiceBankMsb"), bankMsb );
      mVoiceList->setInt( voiceRow, QStringLiteral("voiceBankLsb"), bankLsb );
      mVoiceList->setInt( voiceRow, QStringLiteral("voiceProgram"), prog );
      mVoiceList->setInt( voiceRow, QStringLiteral("voiceId"), id );
      }
    else return false;
    }
  else {
    mVoiceList->setInt( voiceRow, QStringLiteral("voiceBankMsb"), bankMsb );
    mVoiceList->setInt( voiceRow, QStringLiteral("voiceBankLsb"), bankLsb );
    mVoiceList->setInt( voiceRow, QStringLiteral("voiceProgram"), prog );
    mVoiceList->setInt( voiceRow, QStringLiteral("voiceId"), -1 );
    }
  return true;
  }



void SfSynth::applyPresetFromFont(int voiceRow, SoundFontPtr soundFontPtr, int preset)
  {
  //Assign textual name
  mVoiceList->setString( voiceRow, QStringLiteral("voiceSoundFontPresetName"), soundFontPtr->presetName(preset) );
  //..and preset index
  mVoiceList->setInt( voiceRow, QStringLiteral("voiceSoundFontPreset"), preset );
  //By default visual voice name is a same as preset if it previously not assigned
  if( mVoiceList->asString( voiceRow, QStringLiteral("voiceName")).isEmpty() )
    mVoiceList->setString( voiceRow, QStringLiteral("voiceName"), soundFontPtr->presetName(preset) );

  //Because font and/or preset changed we must rebuild all appropriate presets
  int id = mVoiceList->asInt( voiceRow, QStringLiteral("voiceId") );
  if( mPresetCache.contains(id) )
    mPresetCache.object(id)->build( id, mVoiceList->asString( voiceRow, QStringLiteral("voiceName")), soundFontPtr, preset );
  for( int i = 0; i < 16; i++ )
    if( mChannels[i].voiceId() == id )
      channelSetVoiceRow( i, voiceRow );
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

