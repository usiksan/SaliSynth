/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
*/
#include "synthConfig.h"
#include "audioOut/SoundBufferIODevice.h"

#include "midiInput/MidiInput.h"

#include "soundFont/SoundFont.h"
#include "soundFont/SoundFontMap.h"
#include "soundFont/SfSynthPreset.h"

#include "midiFile/MidiFile.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QDebug>

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

extern QAudioOutput *audio;
QAudioOutput *audio;

int main(int argc, char *argv[])
  {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
  engine.load(url);


  //Create audio device for output synthesed audio stream
  QAudioFormat format;
  // Set up the format, eg.
  format.setSampleRate(SAMPLES_PER_SECOND);
  format.setChannelCount(1);
  format.setSampleSize(16);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);

  QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
  if( !info.isFormatSupported(format) ) {
    qWarning() << "Raw audio format not supported by backend, cannot play audio.";
    return 0;
    }

  audio = new QAudioOutput(format, nullptr);
  //Sound buffer - is audio stream source
  SoundBufferIODevice *device = new SoundBufferIODevice();
  device->open( QIODevice::ReadOnly );
  audio->setNotifyInterval(10);
  audio->setBufferSize(1920*4);

  //SoundPolyphonyManager::mSoundPolyphony[10].setPeriod(88);
  //SoundPolyphonyManager::mSoundPolyphony[50].setPeriod(130);

  qDebug() << "buffer size" << audio->bufferSize() << "notify interval" << audio->notifyInterval() << " period size" << audio->periodSize()
           << " bytes free" << audio->bytesFree();
  audio->start( device );

  qDebug() << "buffer size" << audio->bufferSize() << "notify interval" << audio->notifyInterval() << " period size" << audio->periodSize()
           << " bytes free" << audio->bytesFree();

  //Start midi synthesator
  SfSynthPreset *midiOut = new SfSynthPreset();
//  SfSynthTrack track;

  //Synthesator output connect to SoundBufferIODevice
  midiOut->connect( midiOut, &SfSynthPreset::noteOn, device, &SoundBufferIODevice::addNote );

  //Start midi keyboard
  QThread *midiThread = new QThread();
  MidiInput *midiInput = new MidiInput( midiThread );
  midiInput->connect( midiInput, &MidiInput::midi, midiOut, &SfSynthPreset::midi );
  midiThread->start();

  SoundFontMap::init();

  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Fleita Main.SF2", 0, 0, 0 );
  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Fleita Main.SF2", 1, 0, 1 );
  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Fleita Main.SF2", 2, 0, 2 );
  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Fleita Main.SF2", 3, 0, 3 );

//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Organs-Strings-V1.0.sf2", 0, 0, 0 );
//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Organs-Strings-V1.0.sf2", 1, 0, 1 );
//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Organs-Strings-V1.0.sf2", 2, 0, 2 );
//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Organs-Strings-V1.0.sf2", 3, 0, 3 );

//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Giga Piano.sf2", 0, 0, 4 );
//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Giga Piano.sf2", 1, 0, 5 );
//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Giga Piano.sf2", 2, 0, 6 );
//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Giga Piano.sf2", 3, 0, 7 );

//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Nice-4-Bass-V1.5.sf2", 0, 0, 8 );
//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Nice-4-Bass-V1.5.sf2", 1, 0, 9 );
//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Nice-4-Bass-V1.5.sf2", 2, 0, 10 );
//  SoundFontMap::append( "/home/dial/work/SaliSynth/sf2/Nice-4-Bass-V1.5.sf2", 3, 0, 11 );

//  SoundFontMap::append( "/home/asibilev/work/SaliSynth/sf2/Piano Grand.SF2", 0, 0, 0 );
//  SoundFontMap::append( "/home/asibilev/work/SaliSynth/sf2/Piano Grand.SF2", 1, 0, 1 );
//  SoundFontMap::append( "/home/asibilev/work/SaliSynth/sf2/Piano Grand.SF2", 2, 0, 2 );
//  SoundFontMap::append( "/home/asibilev/work/SaliSynth/sf2/Piano Grand.SF2", 3, 0, 3 );

  midiOut->programm(0);

  //midiOut->midi( 0x10, 60, 120 );
//  SoundFont font;
//  font.read( "/home/dial/work/SaliSynth/sf2/Piano Grand.SF2" );

//  MidiFile midi;
//  midi.read( "/home/dial/midi/white_dove.mid" );

  return app.exec();
  }
