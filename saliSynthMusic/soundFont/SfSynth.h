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
    QMap<QString,SoundFontWeakPtr> mSoundFontMap;

    SvQmlJsonModel                *mModel;
    bool                           mMidiConnected;

    Q_PROPERTY(SvQmlJsonModel* model READ getModel WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(bool midiConnected READ getMidiConnected NOTIFY midiConnectedChanged )
  public:
    explicit SfSynth(QObject *parent = nullptr);

    //Model access
    SvQmlJsonModel *getModel() const { return mModel; }
    void            setModel( SvQmlJsonModel *md );

    //midiConnected access
    bool            getMidiConnected() const { return mMidiConnected; }

    Q_INVOKABLE QStringList presetList( int programm );

    Q_INVOKABLE QString     soundFontPath() const;
  signals:
    void noteOn( SfSynthNote *note );

    void modelChanged();

    void midiConnectedChanged();

  public slots:
    void midiSlot( quint8 cmd, quint8 data0, quint8 data1 );

    void setProgramm( int channel, int programm );

    void midiConnection( bool on );

    void applySoundFont(int programm, const QString soundFont, int preset );

    void applyPreset( int programm, int preset );

  private:
    void addModelRecord( int index, const QString title );
  };

#endif // SFSYNTH_H
