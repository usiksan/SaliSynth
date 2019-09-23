/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SoundFont.h"

#include <QDebug>

SoundFont::SoundFont()
  {

  }

SoundFont::~SoundFont()
  {

  }



void SoundFont::clear()
  {
  //qDeleteAll(mSamples);
  mSamples.clear();
  mSampleHeaders.clear();
  }



bool SoundFont::read(const QString fname)
  {
  IffReader reader(fname,true);
  if( !reader.isValid() )
    return false;

  if( reader.readChunk( "RIFF", [this] ( IffReader &reader ) -> bool { return readRiff(reader); } ) ) {
    mFileName = fname;
    return true;
    }
  return false;
  }




bool SoundFont::readRiff(IffReader &reader)
  {
  return reader.needFour( "sfbk" ) && readSfbk(reader);
  }



bool SoundFont::readSfbk(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    IffReader chunk = reader.getChunk();
    if( !chunk.compareChunkName("LIST") )
      return false;

    switch( chunk.getUint32be() ) {
      case IFF_FOUR('I','N','F','O') :
        if( !readInfo( chunk ) ) return false;
        break;
      case IFF_FOUR('s','d','t','a') :
        if( !readSdta( chunk ) ) return false;
        break;
      case IFF_FOUR('p','d','t','a') :
        if( !readPdta( chunk ) ) return false;
        break;
      }
    }
  return true;
  }



bool SoundFont::readInfo(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    IffReader chunk = reader.getChunk();
    qDebug() << "INFO" << chunk.name();
    switch( chunk.chunkFour() ) {
//      case IFF_FOUR('i','f','i','l') :
//        //Version
//        qDebug() << "ifil" << chunk.getUint16le() << '.' << chunk.getUint16le();
//        break;
//      case IFF_FOUR('i','s','n','g') :
//        qDebug() << "isng";
//        break;
//      case IFF_FOUR('I','N','A','M') :
//        qDebug() << "INAM";
//        break;
//      case IFF_FOUR('i','r','o','m') :
//        qDebug() << "INAM";
//        break;
//      case IFF_FOUR('i','v','e','r') :
//        break;
//      case IFF_FOUR('I','C','R','D') :
//        break;
//      case IFF_FOUR('I','E','N','G') :
//        break;
//      case IFF_FOUR('I','P','R','D') :
//        break;
//      case IFF_FOUR('I','C','O','P') :
//        break;
//      case IFF_FOUR('I','C','M','T') :
//        break;
//      case IFF_FOUR('I','S','F','T') :
//        break;
      }
    }
  return true;
  }



bool SoundFont::readSdta(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    IffReader chunk = reader.getChunk();
    qDebug() << "sdta" << chunk.name();
    switch( chunk.chunkFour() ) {
      case IFF_FOUR('s','m','p','l') :
        if( !readSmpl(chunk) ) return false;
        break;
//      case IFF_FOUR('s','m','2','4') :
//        break;
      }
    }
  return true;
  }


bool SoundFont::readPdta(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    IffReader chunk = reader.getChunk();
    qDebug() << "pdta" << chunk.name();
    switch( chunk.chunkFour() ) {
      case IFF_FOUR('p','h','d','r') :
        if( !readPhdr(chunk) ) return false;
        break;
      case IFF_FOUR('p','b','a','g') :
        if( !readPbag(chunk) ) return false;
        break;
      case IFF_FOUR('p','m','o','d') :
        if( !readPmod(chunk) ) return false;
        break;
      case IFF_FOUR('p','g','e','n') :
        if( !readPgen( chunk ) ) return false;
        break;
      case IFF_FOUR('i','n','s','t') :
        if( !readInst( chunk ) ) return false;
        break;
      case IFF_FOUR('i','b','a','g') :
        if( !readIbag( chunk ) ) return false;
        break;
      case IFF_FOUR('i','m','o','d') :
        if( !readImod( chunk ) ) return false;
        break;
      case IFF_FOUR('i','g','e','n') :
        if( !readIgen( chunk ) ) return false;
        break;
      case IFF_FOUR('s','h','d','r') :
        if( !readShdr( chunk ) ) return false;
        break;
      }
    }
  return true;
  }



bool SoundFont::readSmpl(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    qint16 sample = reader.getInt16le();
    mSamples.append(sample);
    }
  return true;
  }



bool SoundFont::readShdr(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfSample sm;
    reader.readChars( sm.achSampleName, 20 );
    sm.achSampleName[20] = 0;
    sm.dwStart           = reader.getUint32le();
    sm.dwEnd             = reader.getUint32le();
    sm.dwStartloop       = reader.getUint32le();
    sm.dwEndloop         = reader.getUint32le();
    sm.dwSampleRate      = reader.getUint32le();
    sm.byOriginalPitch   = reader.getUint8();
    sm.chPitchCorrection = reader.getInt8();
    sm.wSampleLink       = reader.getUint16le();
    sm.sfSampleType      = reader.getUint16le();
    qDebug() << "header" << sm.achSampleName;
    mSampleHeaders.append( sm );
    }
  return true;
  }




bool SoundFont::readInst(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfInstrument inst;
    reader.readChars( inst.achInstrumentName, 20 );
    inst.achInstrumentName[20] = 0;
    inst.wInstumentBagIndex = reader.getUint16le();
    qDebug() << "inst" << inst.achInstrumentName << inst.wInstumentBagIndex;
    if( mInstruments.count() )
      mInstruments.last().mLastBagIndex = inst.wInstumentBagIndex - 1;
    mInstruments.append( inst );
    }
  return true;
  }




bool SoundFont::readIbag(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfBag bag;
    bag.mGeneratorStartIndex = reader.getUint16le();
    bag.mModulatorStartIndex = reader.getUint16le();
    qDebug() << "bag" << bag.mGeneratorStartIndex << bag.mModulatorStartIndex;
    mInstrumentBags.append( bag );
    }
  return true;
  }



bool SoundFont::readImod(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfModulator mod;
    mod.mModSrcOper = reader.getUint16le();
    mod.mModDstOper = reader.getUint16le();
    mod.mModAmount = reader.getInt16le();
    mod.mModAmtSrcOper = reader.getUint16le();
    mod.mModTransOper = reader.getUint16le();
    qDebug() << "mod" << mod.mModSrcOper << mod.mModDstOper << mod.mModAmount << mod.mModAmtSrcOper << mod.mModTransOper;
    mInstrumentModulators.append( mod );
    }
  return true;
  }




bool SoundFont::readIgen(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfGeneratorParam gen;
    gen.mParamIndex = reader.getUint16le();
    gen.mParamValue = reader.getUint16le();
    qDebug() << "gen" << gen.mParamIndex << gen.mParamValue;
    mInstrumentGenerators.append( gen );
    }
  return true;
  }




bool SoundFont::readPhdr(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfPreset hdr;
    reader.readChars( hdr.achPresetName, 20 );
    hdr.achPresetName[20] = 0;
    hdr.wPreset         = reader.getUint16le();           //Unique preset and bank combination
    hdr.wBank           = reader.getUint16le();
    hdr.wPresetBagIndex = reader.getUint16le();
    //Reserved for feature
    hdr.dwLibrary      = reader.getUint32le();
    hdr.dwGentre       = reader.getUint32le();
    hdr.dwMorphology   = reader.getUint32le();
    qDebug() << "Preset" << hdr.achPresetName << hdr.wPreset << hdr.wBank << hdr.wPresetBagIndex;
    mPresets.append( hdr );
    }
  return true;
  }



bool SoundFont::readPbag(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfBag bag;
    bag.mGeneratorStartIndex = reader.getUint16le();
    bag.mModulatorStartIndex = reader.getUint16le();
    qDebug() << "bag" << bag.mGeneratorStartIndex << bag.mModulatorStartIndex;
    mPresetBags.append( bag );
    }
  return true;
  }




bool SoundFont::readPgen(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfGeneratorParam gen;
    gen.mParamIndex = reader.getUint16le();
    gen.mParamValue = reader.getUint16le();
    qDebug() << "gen" << gen.mParamIndex << gen.mParamValue;
    mPresetGenerators.append( gen );
    }
  return true;
  }




bool SoundFont::readPmod(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfModulator mod;
    mod.mModSrcOper = reader.getUint16le();
    mod.mModDstOper = reader.getUint16le();
    mod.mModAmount = reader.getInt16le();
    mod.mModAmtSrcOper = reader.getUint16le();
    mod.mModTransOper = reader.getUint16le();
    qDebug() << "mod" << mod.mModSrcOper << mod.mModDstOper << mod.mModAmount << mod.mModAmtSrcOper << mod.mModTransOper;
    mPresetModulators.append( mod );
    }
  return true;
  }


