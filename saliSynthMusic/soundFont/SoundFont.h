/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   SoundFont2 file
*/
#ifndef SOUNDFONT_H
#define SOUNDFONT_H

#include "iff/IffReader.h"
#include "SfSample.h"
#include "SfInstrument.h"
#include "SfPreset.h"
#include <QVector>

//Instrument descriptor
struct SfInstrument
  {
    char    achInstrumentName[20]; //Instrument name
    quint16 wInstumentBagIndex;    //Instrument first zone
    quint16 mLastBagIndex;         //Instrument last zone
  };

using SfInstrumentVector = QVector<SfInstrument>;


//Zones descriptor
struct SfInstBag {
    quint16 wInstGenNdx;
    quint16 wInstModNdx;
  };

using SfInstBagVector = QVector<SfInstBag>;

//Instument modulator
struct SfInstMod {
  quint16 sfModSrcOper;
  quint16 sfModDstOper;
  qint16  modAmount;
  quint16 sfModAmtSrcOper;
  quint16 sfModTransOper;
  };

using SfInstModVector = QVector<SfInstMod>;


//Instrument generator
struct SfInstGen {
    quint16 sfGenOper;
    quint16 genAmount;
  };


using SfInstGenVector = QVector<SfInstGen>;


class SoundFont
  {
    QVector<qint16> mSamples;
    SfSamleVector   mSampleHeaders;
    SfInstrumentVector mInstruments;
    SfInstBagVector    mInstBag;
    SfInstModVector    mInstMod;
    SfInstGenVector    mInstGen;
  public:
    SoundFont();
    ~SoundFont();

    void clear();

    bool read( const QString fname );

  private:
    bool readRiff( IffReader &reader );

    bool readSfbk( IffReader &reader );

    bool readInfo( IffReader &reader );

    bool readSdta( IffReader &reader );

    bool readPdta( IffReader &reader );

    bool readSmpl( IffReader &reader );

    bool readShdr( IffReader &reader );

    bool readInst( IffReader &reader );

    bool readIbag( IffReader &reader );

    bool readImod( IffReader &reader );

    bool readIgen( IffReader &reader );
  };

#endif // SOUNDFONT_H
