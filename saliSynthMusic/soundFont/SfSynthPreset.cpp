#include "SfSynthPreset.h"
#include "SoundFontMap.h"

#include <math.h>
#include <QDebug>

SfSynthPreset::SfSynthPreset(QObject *parent) :
  QObject(parent)
  {
  for( int i = 0; i < 128; i++ )
    mNotes[i].setNote(i);
  }

void SfSynthPreset::midi(quint8 cmd, quint8 data0, quint8 data1)
  {
  cmd = ((cmd >> 4) & 0x7);
  switch( cmd ) {
    case 0 :
      //Note off [note pressure]
      mNotes[data0].noteOff( data1 );
      break;
    case 1 :
      //Note on [note pressure]
      if( mNotes[data0].noteOn( data1 ) )
        emit noteOn( mNotes + data0 );
      break;
    case 2 :
      //Polyphonic key pressure [note pressure]
      break;
    case 3 :
      //Control change (controller number) (controller value)
      break;
    case 4 :
      //Programm change (programm number)
      programm( data0 );
      break;
    case 5 :
      //Channel pressure (pressure)
      break;
    case 6 :
      //Pitch wheel change (lsb) (msb)
      break;

    }
  if( cmd != 7 )
    qDebug() << "cmd " << cmd << data0 << data1;
  }





inline int delay( quint16 time ) {
  double tm = static_cast<qint16>(time);
  double sec = pow( 2.0, tm / 1200.0 );
  return static_cast<int>( sec * 48000.0 );
  }


void SfSynthPreset::build(SoundFontPtr soundFont, int preset)
  {
  //Stop all notes
  for( int i = 0; i < 128; i++ )
    mNotes[i].clear();

  //Build all notes
  mSoundFontPtr = soundFont;

  mSoundFontPtr->buildPreset( preset, [this] ( quint16 *generator, const SfSample &sam, qint16 *samples ) ->bool {
    //Note range
    int maxNote = generator[sfpiKeyRange] >> 8;
    int minNote = generator[sfpiKeyRange] & 0xff;

    //Samples bounds
    int startSample = static_cast<int>(sam.dwStart) + static_cast<qint16>(generator[sfpiStartAddressOffset]) + (static_cast<qint16>(generator[sfpiStartAddressCoarseOffset]) << 16);
    int endSample   = static_cast<int>(sam.dwEnd) + static_cast<qint16>(generator[sfpiEndAddressOffset]) + (static_cast<qint16>(generator[sfpiEndAddressCoarseOffset]) << 16);
    int startLoop   = static_cast<int>(sam.dwStartloop) + static_cast<qint16>(generator[sfpiStartLoopAddressOffset]) + (static_cast<qint16>(generator[sfpiStartLoopCoarseOffset]) << 16);
    int endLoop     = static_cast<int>(sam.dwEndloop) + static_cast<qint16>(generator[sfpiEndLoopAddressOffset]) + (static_cast<qint16>(generator[sfpiEndLoopCoarseOffset]) << 16);

    samples   += startSample;
    endSample -= startSample;
    startLoop -= startSample;
    endLoop   -= startSample;


    int delayVolEnv = delay( generator[sfpiDelayVolEnv] );
    int attackVolEnv = delay( generator[sfpiAttackVolEnv] );
    int holdVolEnv = delay( generator[sfpiHoldVolEnv] );
    int decayVolEnv = delay( generator[sfpiDecayVolEnv] );
    int releaseVolEnv = delay( generator[sfpiReleaseVolEnv] );

    //Build each note track from range
    while( minNote <= maxNote )
      if( !mNotes[minNote++].addTrack( generator, sam, samples, endSample, startLoop, endLoop, delayVolEnv, attackVolEnv, holdVolEnv, decayVolEnv, releaseVolEnv ) )
        return false;
    return true;
    });
  }




void SfSynthPreset::programm(quint8 prg)
  {
  SoundFontMap map = SoundFontMap::map( 0, prg );
  build( map.mSoundFontPtr, map.mSoundFontPreset );
  }