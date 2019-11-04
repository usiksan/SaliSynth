#ifndef SFSYNTH_H
#define SFSYNTH_H

#include "SfSynthPreset.h"

#include <QObject>
#include <QMap>
#include <QVector>

class SfSynth : public QObject
  {
    Q_OBJECT

    SfSynthPresetPtr           mChannels[16];
    QMap<int,SfSynthPresetPtr> mSynthPresetMap;
  public:
    explicit SfSynth(QObject *parent = nullptr);

  signals:
    void noteOn( SfSynthNote *note );

  public slots:
    void midi( quint8 cmd, quint8 data0, quint8 data1 );
  };

#endif // SFSYNTH_H
