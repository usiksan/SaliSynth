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

  return reader.readChunk( "RIFF", [this] ( IffReader &reader ) -> bool { return readRiff(reader); } );
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
        if( !readSmpl(chunk) ) return false;
        break;
      case IFF_FOUR('p','b','a','g') :
        break;
      case IFF_FOUR('p','m','o','d') :
        break;
      case IFF_FOUR('p','g','e','n') :
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
    SfInstBag bag;
    bag.wInstGenNdx = reader.getUint16le();
    bag.wInstModNdx = reader.getUint16le();
    qDebug() << "bag" << bag.wInstGenNdx << bag.wInstModNdx;
    mInstBag.append( bag );
    }
  return true;
  }



bool SoundFont::readImod(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfInstMod mod;
    mod.sfModSrcOper = reader.getUint16le();
    mod.sfModDstOper = reader.getUint16le();
    mod.modAmount = reader.getInt16le();
    mod.sfModAmtSrcOper = reader.getUint16le();
    mod.sfModTransOper = reader.getUint16le();
    qDebug() << "mod" << mod.sfModSrcOper << mod.sfModDstOper << mod.modAmount << mod.sfModAmtSrcOper << mod.sfModTransOper;
    mInstMod.append( mod );
    }
  return true;
  }




bool SoundFont::readIgen(IffReader &reader)
  {
  while( !reader.isEnd() ) {
    SfInstGen gen;
    gen.sfGenOper = reader.getUint16le();
    gen.genAmount = reader.getUint16le();
    qDebug() << "gen" << gen.sfGenOper << gen.genAmount;
    mInstGen.append( gen );
    }
  return true;
  }


