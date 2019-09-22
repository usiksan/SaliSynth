/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   SoundFont2
   Sample
*/
#ifndef SFSAMPLE_H
#define SFSAMPLE_H

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

using SfSamleVector = QVector<SfSample>;

#endif // SFSAMPLE_H
