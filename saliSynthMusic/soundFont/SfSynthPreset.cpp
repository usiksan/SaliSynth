#include "SfSynthPreset.h"
#include "SoundFontMap.h"

#include <QDebug>

SfSynthPreset::SfSynthPreset(QObject *parent) : QObject(parent)
  {

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



void SfSynthPreset::build(SoundFontPtr soundFont, int preset)
  {
  //Stop all notes
  for( int i = 0; i < 128; i++ )
    mNotes[i].clear();

  //Build all notes
  mSoundFontPtr = soundFont;

  }




void SfSynthPreset::programm(quint8 prg)
  {
  SoundFontMap map = SoundFontMap::map( 0, prg );
  build( map.mSoundFontPtr, map.mSoundFontPreset );
  }
