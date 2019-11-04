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
#include "soundFont/SfSynthPreset.h"
#include "soundFont/SfSynth.h"

#include "midiFile/MidiFile.h"

#include "SvQml/SvQmlUtils.h"

//json-files
#include "SvQml/SvQmlJsonFile.h"
#include "SvQml/SvQmlJsonString.h"
#include "SvQml/SvQmlJsonHistory.h"
#include "SvQml/SvQmlJsonModel.h"

//Help provider
#include "SvQml/SvQmlHtmlImageProvider.h"


#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
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


  //Utilites
  qmlRegisterType<SvQmlUtils>               ("SvQml", 1, 0, "SvQmlUtils" );

  //json-files
  qmlRegisterType<SvQmlJsonFile>            ("SvQml", 1, 0, "SvQmlJsonFile" );
  qmlRegisterType<SvQmlJsonString>          ("SvQml", 1, 0, "SvQmlJsonString" );
  qmlRegisterType<SvQmlJsonHistory>         ("SvQml", 1, 0, "SvQmlJsonHistory" );
  qmlRegisterType<SvQmlJsonModel>           ("SvQml", 1, 0, "SvQmlJsonModel" );


  QQmlApplicationEngine engine;

  //Help provider
  SvQmlHtmlImageProvider *guideImageProvider = new SvQmlHtmlImageProvider();
  engine.addImageProvider( "guideImageProvider", guideImageProvider );
  engine.rootContext()->setContextProperty("guideImageProvider", guideImageProvider);

  engine.rootContext()->setContextProperty( "version", QVariant(VERSION) );


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

  audio->start( device );


  //Start midi synthesator
  SfSynth *synth = new SfSynth();

  //Synthesator output connect to SoundBufferIODevice
  synth->connect( synth, &SfSynth::noteOn, device, &SoundBufferIODevice::addNote );

  //Start midi keyboard
  QThread *midiThread = new QThread();
  MidiInput *midiInput = new MidiInput( midiThread );
  midiInput->connect( midiInput, &MidiInput::midi, synth, &SfSynth::midi );
  midiThread->start();


  //Inject synth to visual subsystem
  engine.rootContext()->setContextProperty( "synth", synth );


  const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
  engine.load(url);

//  MidiFile midi;
//  midi.read( "/home/dial/midi/white_dove.mid" );

  return app.exec();
  }
