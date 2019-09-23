#ifndef SOUNDFONTMAP_H
#define SOUNDFONTMAP_H

#include "SoundFont.h"


struct SoundFontMap
  {
    SoundFontPtr mSoundFontPtr;
    int          mSoundFontPreset;

    int          mBank;
    int          mPreset;

    static void         init();
    static SoundFontMap map( int bank, int preset );
    static void         append( const QString soundFontFileName, int soundFontPreset, int bank, int preset );
    static void         remove( int bank, int preset );
    static int          count();
    static SoundFontMap at(int i);
  };


#endif // SOUNDFONTMAP_H
