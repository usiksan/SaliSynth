#ifndef SFSYNTHPRESET_H
#define SFSYNTHPRESET_H

#include "SfSynthNote.h"
#include "SoundFont.h"

#include <QObject>


class SfSynthPreset : public QObject
  {
    Q_OBJECT

    SfSynthNote  mNotes[128];
    SoundFontPtr mSoundFontPtr;
    int          mProgramm;
  public:
    explicit SfSynthPreset(QObject *parent = nullptr);

    int programm() const { return mProgramm; }


  signals:
    void noteOn( SfSynthNote *note );

  public slots:
    void midi( quint8 cmd, quint8 data0, quint8 data1 );

    void build( SoundFontPtr soundFont, int preset );

    void programm( quint8 prg );
  };

using SfSynthPresetPtr = QSharedPointer<SfSynthPreset>;

#endif // SFSYNTHPRESET_H
