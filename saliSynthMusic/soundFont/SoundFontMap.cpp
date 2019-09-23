#include "SoundFontMap.h"
#include <QList>
#include <QMap>

static QList<SoundFontMap> soundFontMapList;
static QMap<int,int>       soundFontMap;

inline int fontKey( int bank, int preset ) { return preset * 128 + bank; }

void SoundFontMap::init()
  {
  if( soundFontMapList.count() == 0 ) {
    //Build default map
    SoundFontMap map;
    map.mSoundFontPtr.reset( new SoundFont );
    map.mSoundFontPreset = 0;
    map.mBank = 0;
    map.mPreset = 0;
    soundFontMapList.append( map );
    }
  }



SoundFontMap SoundFontMap::map(int bank, int preset)
  {
  init();
  return soundFontMapList.at( soundFontMap.value( fontKey( bank, preset ) ) );
  }




void SoundFontMap::append(const QString soundFontFileName, int soundFontPreset, int bank, int preset)
  {
  init();
  int key = fontKey( bank, preset );
  SoundFontPtr soundFontPtr;
  //Try to find always loaded sound font
  for( int i = 0; i < soundFontMapList.count(); i++ )
    if( soundFontMapList.at(i).mSoundFontPtr->fileName().endsWith(soundFontFileName) ) {
      soundFontPtr = soundFontMapList.at(i).mSoundFontPtr;
      break;
      }

  if( soundFontPtr.isNull() ) {
    soundFontPtr.reset( new SoundFont );
    soundFontPtr->read( soundFontFileName );
    }

  if( soundFontMap.contains( key ) ) {
    //Remove previous assigning
    int i = soundFontMap.value( fontKey( bank, preset ) );
    soundFontMapList.removeAt(i);
    }

  SoundFontMap map;
  map.mSoundFontPtr    = soundFontPtr;
  map.mSoundFontPreset = soundFontPreset;
  map.mBank            = bank;
  map.mPreset          = preset;

  soundFontMap.insert( key, soundFontMapList.count() );
  soundFontMapList.append( map );
  }
