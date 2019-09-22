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
#include "audioOut/SoundPolyphonyManager.h"
#include "audioOut/SoundMidiOut.h"

#include "midiInput/MidiInput.h"

#include "soundFont/SoundFont.h"

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
  SoundMidiOut *midiOut = new SoundMidiOut();


  //Start midi keyboard
  QThread *midiThread = new QThread();
  MidiInput *midiInput = new MidiInput( midiThread );
  midiInput->connect( midiInput, &MidiInput::midi, midiOut, &SoundMidiOut::midi );
  midiThread->start();

  SoundFont font;
  font.read( "/home/dial/midi/25 Piano Soundfonts/Grand Piano.sf2" );

  return app.exec();
  }
