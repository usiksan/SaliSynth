/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   SoundFont based soft sinthesizer
*/
#ifndef SFSYNTH_H
#define SFSYNTH_H

#include "SoundFont.h"
#include "SfSynthVoice.h"
#include "SfSynthMetronom.h"
#include "SvQml/SvQmlJsonModel.h"

#include <QObject>
#include <QMap>
#include <QCache>

class SfSynth : public QObject
  {
    Q_OBJECT

    SfSynthVoice                   mChannels[16];   //! Preset of channel
    QMap<QString,SoundFontWeakPtr> mSoundFontMap;   //! Used sound font map
    QCache<int,SfSynthVoice>       mPresetCache;    //! Cached presets

    SvQmlJsonModel                *mVoiceList;      //! Model of registered voice list. Each voice mapped to bank and programm
    SvQmlJsonModel                *mChannelList;    //! Model of channels
    bool                           mMidiConnected;  //! Flag for indicate midi keyboard connection

    SfSynthMetronom                mMetronome;       //! Metronom note
    bool                           mMetronomeMute;   //! Metronom mute

    Q_PROPERTY(SvQmlJsonModel* voiceList READ voiceList WRITE setVoiceList NOTIFY voiceListChanged)
    Q_PROPERTY(SvQmlJsonModel* channelList READ channelList WRITE setChannelList NOTIFY channelListChanged)
    Q_PROPERTY(bool midiConnected READ getMidiConnected NOTIFY midiConnectedChanged )

    Q_PROPERTY(QString leftVoice READ leftVoice NOTIFY leftVoiceChanged)
    Q_PROPERTY(QString rightMainVoice READ rightMainVoice NOTIFY rightMainVoiceChanged)
    Q_PROPERTY(QString rightSlaveVoice READ rightSlaveVoice NOTIFY rightSlaveVoiceChanged)

    Q_PROPERTY(int metronomVolume READ metronomVolume WRITE setMetronomVolume NOTIFY metronomVolumeChanged)
    Q_PROPERTY(bool metronomMute READ metronomMute WRITE setMetronomMute NOTIFY metronomMuteChanged)
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

    //Metronom volume access
    int             metronomVolume() const { return mMetronome.getVolume(); }
    void            setMetronomVolume( int volume );


    //Metronom mute access
    bool            metronomMute() const { return mMetronomeMute; }
    void            setMetronomMute( bool mute );


    Q_INVOKABLE QStringList presetList(int voiceRow );

    Q_INVOKABLE QString     soundFontPath() const;

    Q_INVOKABLE QString     midiPath() const;

    Q_INVOKABLE QString     stylePath() const;

    Q_INVOKABLE QString     channelVoiceName( int channel ) const { return mChannels[channel].voiceName(); }

    Q_INVOKABLE int         channelVoiceId( int channel ) const { return mChannels[channel].voiceId(); }

    Q_INVOKABLE bool        containsVoice( int bankMsb, int bankLsb, int midiProgram );

    Q_INVOKABLE int         voiceRow( int bankMsb, int bankLsb, int midiProgram );

    Q_INVOKABLE int         voiceRowById( int voiceId ) const;

    Q_INVOKABLE QString     voiceName( int voiceId ) const;

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

    void metronomVolumeChanged();
    void metronomMuteChanged();

  public slots:
    void midiSlot( quint8 cmd, quint8 data0, quint8 data1 );

    void metronome();

    void setProgramm( int channel, int programm );

    void midiConnection( bool on );

    void applySoundFont( int voiceRow, const QString soundFont, int preset );

    void applyPreset( int voiceRow, int preset );

    void channelSetVoiceId( quint8 channel, int voiceId );

    void channelSetVoiceRow( int channel, int voiceRow );

    void voiceAdd();

    void voiceInsert( int row );

    void voiceDuplicate( int voiceRow );

    void voiceFontExtractAll( int voiceRow );

    bool voiceSettings( int voiceRow, int bankMsb, int bankLsb, int prog );
  private:
    void applyPresetFromFont( int voiceRow, SoundFontPtr fontPtr, int preset );

    void syncroChannelsWithProgramm( int programm );

    SoundFontPtr soundFont( const QString fontName );
  };

#endif // SFSYNTH_H
