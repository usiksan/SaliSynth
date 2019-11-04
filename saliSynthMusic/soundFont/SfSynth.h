#ifndef SFSYNTH_H
#define SFSYNTH_H

#include "SoundFont.h"
#include "SfSynthPreset.h"
#include "SvQml/SvQmlJsonModel.h"

#include <QObject>
#include <QMap>
#include <QVector>

class SfSynth : public QObject
  {
    Q_OBJECT

    SfSynthPresetPtr               mChannels[16];
    SfSynthPresetPtr               mProgramms[128];
    QMap<QString,SoundFontPtr>     mSoundFontMap;

    SvQmlJsonModel                *mModel;

    Q_PROPERTY(SvQmlJsonModel* model READ getModel WRITE setModel NOTIFY modelChanged)
  public:
    explicit SfSynth(QObject *parent = nullptr);

    //Model access
    SvQmlJsonModel *getModel() const { return mModel; }
    void            setModel( SvQmlJsonModel *md );

    Q_INVOKABLE QStringList presetList(const QString soundFont );
  signals:
    void noteOn( SfSynthNote *note );

    void modelChanged();

  public slots:
    void midi( quint8 cmd, quint8 data0, quint8 data1 );

    void applySoundFont(int programm, const QString soundFont, int preset );

  private:
    void addModelRecord( int index, const QString title );
  };

#endif // SFSYNTH_H
