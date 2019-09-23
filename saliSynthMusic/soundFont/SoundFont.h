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
#include <QVector>

struct SfSample
  {
    char    achSampleName[21];
    quint32 dwStart;
    quint32 dwEnd;
    quint32 dwStartloop;
    quint32 dwEndloop;
    quint32 dwSampleRate;
    quint8  byOriginalPitch;
    qint8   chPitchCorrection;
    quint16 wSampleLink;
    quint16 sfSampleType;
  };

using SfSampleVector = QVector<SfSample>;




struct SfPreset
  {
    char    achPresetName[21]; //Preset name
    quint16 wPreset;           //Unique preset and bank combination
    quint16 wBank;
    quint16 wPresetBagIndex;
    //Reserved for feature
    quint32 dwLibrary;
    quint32 dwGentre;
    quint32 dwMorphology;
  };

using SfPresetVector = QVector<SfPreset>;




struct SfBag
  {
    quint16 mGeneratorStartIndex; //Start index in generator param vector
    quint16 mModulatorStartIndex; //Start index in modulator param vector
  };

using SfBagVector = QVector<SfBag>;



//One generator parameter description
struct SfGeneratorParam
  {
    quint16 mParamIndex; //Param index - identify param
    quint16 mParamValue; //Param value

    quint16 getUint16() const { return mParamValue; }
    qint16  getInt16() const { return static_cast<qint16>(mParamValue); }
    quint8  getMB() const { return static_cast<quint8>(mParamValue >> 8); }
    quint8  getLB() const { return static_cast<quint8>(mParamValue & 0xff); }
  };

using SfGeneratorParamVector = QVector<SfGeneratorParam>;


//Modulator description
struct SfModulator
  {
    quint16 mModSrcOper;    //Source data of modulator
    quint16 mModDstOper;    //Destignation data of modulator
    qint16  mModAmount;     //Indicating the degree to which the source modulates the destination. A zero value indicates there is no fixed amount.
    quint16 mModAmtSrcOper; //This value indicates the degree to which the source modulates the destination is to be controlled by the specified modulation source.
    quint16 mModTransOper;  //This value indicates that a transform of the specified type will be applied to the modulation source before application to the modulator.
  };

using SfModulatorVector = QVector<SfModulator>;


//Instrument descriptor
struct SfInstrument
  {
    char    achInstrumentName[21]; //Instrument name
    quint16 wInstumentBagIndex;    //Instrument first zone
    quint16 mLastBagIndex;         //Instrument last zone
  };

using SfInstrumentVector = QVector<SfInstrument>;



class SoundFont
  {
    QVector<qint16>        mSamples;
    SfSampleVector         mSampleHeaders;
    SfPresetVector         mPresets;
    SfBagVector            mPresetBags;
    SfModulatorVector      mPresetModulators;
    SfGeneratorParamVector mPresetGenerators;
    SfInstrumentVector     mInstruments;
    SfBagVector            mInstrumentBags;
    SfModulatorVector      mInstrumentModulators;
    SfGeneratorParamVector mInstrumentGenerators;
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

    bool readPhdr( IffReader &reader );

    bool readPbag( IffReader &reader );

    bool readPgen( IffReader &reader );

    bool readPmod( IffReader &reader );
  };

#endif // SOUNDFONT_H
