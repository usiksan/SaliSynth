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

    SfSynthPreset                  mChannels[16];         //Preset of channel
    int                            mChannelsProgramm[16]; //ID of programm of appropriate channel
    SfSynthPreset                  mProgramms[128];
    QMap<QString,SoundFontWeakPtr> mSoundFontMap;

    SvQmlJsonModel                *mModel;
    bool                           mMidiConnected;

    Q_PROPERTY(SvQmlJsonModel* model READ getModel WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(bool midiConnected READ getMidiConnected NOTIFY midiConnectedChanged )
//    Q_PROPERTY(QString channel0Preset READ channel0Preset WRITE setChannel0Preset NOTIFY channel0PresetChanged)
//    Q_PROPERTY(QString channel1Preset READ channel1Preset WRITE setChannel1Preset NOTIFY channel1PresetChanged)
//    Q_PROPERTY(QString channel2Preset READ channel2Preset WRITE setChannel2Preset NOTIFY channel2PresetChanged)
  public:
    explicit SfSynth(QObject *parent = nullptr);

    //Model access
    SvQmlJsonModel *getModel() const { return mModel; }
    void            setModel( SvQmlJsonModel *md );

    //midiConnected access
    bool            getMidiConnected() const { return mMidiConnected; }

    //channel 0 preset access
    QString         channel0Preset() const { return mChannels[0].name(); }

    Q_INVOKABLE QStringList presetList( int programm );

    Q_INVOKABLE QString     soundFontPath() const;

    Q_INVOKABLE QString     channelPresetName( int channel ) const { return mChannels[channel].name(); }

    void                    emitNoteOn( SfSynthNote *note );
    //Q_INVOKABLE QString     channelPreset()
  signals:
    void noteOn( SfSynthNote *note );

    void modelChanged();

    void midiConnectedChanged();

//    void channel0PresetChanged();

//    void channel1PresetChanged();

//    void channel2PresetChanged();

    void channelPresetChanged( int channel, QString presetName );

  public slots:
    void midiSlot( quint8 cmd, quint8 data0, quint8 data1 );

    void setProgramm( int channel, int programm );

    void midiConnection( bool on );

    void applySoundFont(int programm, const QString soundFont, int preset );

    void applyPreset( int programm, int preset );

  private:
    void addModelRecord( int index, const QString title );

    void syncroChannelsWithProgramm( int programm );
  };

#endif // SFSYNTH_H
