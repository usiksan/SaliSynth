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

    Q_PROPERTY(QString leftVoice READ leftVoice NOTIFY leftVoiceChanged)
//    Q_PROPERTY(QString leftStyle READ leftStyle WRITE setLeftStyle NOTIFY leftStyleChanged)
    Q_PROPERTY(QString rightMainVoice READ rightMainVoice NOTIFY rightMainVoiceChanged)
    Q_PROPERTY(QString rightSlaveVoice READ rightSlaveVoice NOTIFY rightSlaveVoiceChanged)
//    Q_PROPERTY(QString leftVoiceId READ leftVoiceId NOTIFY leftVoiceIdChanged)
//    Q_PROPERTY(QString rightMainVoiceId READ rightMainVoiceId NOTIFY rightMainVoiceIdChanged)
//    Q_PROPERTY(QString rightSlaveVoiceId READ rightSlaveVoiceId NOTIFY rightSlaveVoiceIdChanged)
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

    //Left voice access
    QString         leftVoice() const { return mChannels[1].voiceName(); }
//    int             leftVoiceId() const { return mChannels[1].voiceId(); }

    //Left style access
//    QString         leftStyle() const { return mLeftStyle; }
//    void            setLeftStyle( const QString style ) { mLeftStyle = style; emit leftStyleChanged(); }

    //Right main voice access
    QString         rightMainVoice() const { return mChannels[0].voiceName(); }
//    int             rightMainVoiceId() const { return mChannels[0].voiceId(); }
//    void            setRightMainVoice( const QString voice ) { mRightMainVoice = voice; emit rightMainVoiceChanged(); }

    //Right slave voice access
    QString         rightSlaveVoice() const { return mChannels[2].voiceName(); }
//    int             rightSlaveVoiceId() const { return mChannels[2].voiceId(); }
//    void            setRightSlaveVoice( const QString voice ) { mRightSlaveVoice = voice; emit rightSlaveVoiceChanged(); }


    Q_INVOKABLE QStringList presetList(int voiceRow );

    Q_INVOKABLE QString     soundFontPath() const;

    Q_INVOKABLE QString     channelVoiceName( int channel ) const { return mChannels[channel].voiceName(); }

    Q_INVOKABLE int         channelVoiceId( int channel ) const { return mChannels[channel].voiceId(); }

    Q_INVOKABLE bool        containsVoice( int bankMsb, int bankLsb, int midiProgram );

    Q_INVOKABLE int         voiceRow( int bankMsb, int bankLsb, int midiProgram );

    Q_INVOKABLE int         voiceRowById( int voiceId );

    void                    emitNoteOn( SfSynthNote *note );
  signals:
    void noteOn( SfSynthNote *note );

    void voiceListChanged();

    void channelListChanged();

    void midiConnectedChanged();

    void leftVoiceChanged();
    void leftStyleChanged();
    void rightMainVoiceChanged();
    void rightSlaveVoiceChanged();

    void leftVoiceIdChanged();
    void rightMainVoiceIdChanged();
    void rightSlaveVoiceIdChanged();

  public slots:
    void midiSlot( quint8 cmd, quint8 data0, quint8 data1 );

    void setProgramm( int channel, int programm );

    void midiConnection( bool on );

    void applySoundFont( int voiceRow, const QString soundFont, int preset );

    void applyPreset( int voiceRow, int preset );

    void channelSetVoiceId( int channel, int voiceId );

    void channelSetVoiceRow( int channel, int voiceRow );

    void voiceAdd();

    void voiceDuplicate( int voiceRow );

    void voiceFontExtractAll( int voiceRow );

    bool voiceSettings( int voiceRow, int bankMsb, int bankLsb, int prog );
  private:
    void applyPresetFromFont( int voiceRow, SoundFontPtr fontPtr, int preset );

    void syncroChannelsWithProgramm( int programm );

    SoundFontPtr soundFont( const QString fontName );
  };

#endif // SFSYNTH_H
