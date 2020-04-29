#ifndef SFSYNTH_H
#define SFSYNTH_H

#include "SoundFont.h"
#include "SfSynthPreset.h"
#include "SvQml/SvQmlJsonModel.h"

#include <QObject>
#include <QMap>
#include <QCache>

class SfSynth : public QObject
  {
    Q_OBJECT

    SfSynthPreset                  mChannels[16];  //! Preset of channel
    QMap<QString,SoundFontWeakPtr> mSoundFontMap;  //! Used sound font map
    QCache<int,SfSynthPreset>      mPresetCache;   //! Cached presets

    SvQmlJsonModel                *mVoiceList;
    SvQmlJsonModel                *mChannelList;
    bool                           mMidiConnected;

    Q_PROPERTY(SvQmlJsonModel* voiceList READ voiceList WRITE setVoiceList NOTIFY voiceListChanged)
    Q_PROPERTY(SvQmlJsonModel* channelList READ channelList WRITE setChannelList NOTIFY channelListChanged)
    Q_PROPERTY(bool midiConnected READ getMidiConnected NOTIFY midiConnectedChanged )
//    Q_PROPERTY(QString channel0Preset READ channel0Preset WRITE setChannel0Preset NOTIFY channel0PresetChanged)
//    Q_PROPERTY(QString channel1Preset READ channel1Preset WRITE setChannel1Preset NOTIFY channel1PresetChanged)
//    Q_PROPERTY(QString channel2Preset READ channel2Preset WRITE setChannel2Preset NOTIFY channel2PresetChanged)
  public:
    explicit SfSynth(QObject *parent = nullptr);

    //Voice map model access
    SvQmlJsonModel *voiceList() const { return mVoiceList; }
    void            setVoiceList( SvQmlJsonModel *md );

    //Channel list model access
    SvQmlJsonModel *channelList() const { return mChannelList; }
    void            setChannelList( SvQmlJsonModel *md );

    //midiConnected access
    bool            getMidiConnected() const { return mMidiConnected; }

    //channel 0 preset access
    QString         channel0Preset() const { return mChannels[0].name(); }

    Q_INVOKABLE QStringList presetList(int voiceRow );

    Q_INVOKABLE QString     soundFontPath() const;

    Q_INVOKABLE QString     channelPresetName( int channel ) const { return mChannels[channel].name(); }

    Q_INVOKABLE bool        containsVoice( int bankMsb, int bankLsb, int midiProgram );

    Q_INVOKABLE int         voiceRow( int bankMsb, int bankLsb, int midiProgram );

    Q_INVOKABLE int         voiceRowById( int voiceId );

    void                    emitNoteOn( SfSynthNote *note );
  signals:
    void noteOn( SfSynthNote *note );

    void voiceListChanged();

    void channelListChanged();

    void midiConnectedChanged();

  public slots:
    void midiSlot( quint8 cmd, quint8 data0, quint8 data1 );

    void setProgramm( int channel, int programm );

    void midiConnection( bool on );

    void applySoundFont( int voiceRow, const QString soundFont, int preset );

    void applyPreset( int programm, int preset );

    void channelSetVoiceId( int channel, int voiceId );

    void channelSetVoiceRow( int channel, int voiceRow );

    void voiceAdd();

    void voiceDuplicate( int voiceRow );

    bool voiceSettings( int voiceRow, int bankMsb, int bankLsb, int prog );
  private:

    void syncroChannelsWithProgramm( int programm );

    SoundFontPtr soundFont( const QString fontName );
  };

#endif // SFSYNTH_H
