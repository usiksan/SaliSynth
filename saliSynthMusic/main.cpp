/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Entry point
*/
#include "synthConfig.h"
#include "audioOut/SoundBufferIODevice.h"

#include "midiInput/MidiInput.h"

#include "soundFont/SoundFont.h"
#include "soundFont/SfSynthPreset.h"
#include "soundFont/SfSynth.h"

#include "objects/MidiProcessor.h"

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
  //=============================================================================
  //        Application setup
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);


  //=============================================================================
  //        QML setup
  //Utilites
  qmlRegisterType<SvQmlUtils>               ("SvQml", 1, 0, "SvQmlUtils" );

  //json-files
  qmlRegisterType<SvQmlJsonFile>            ("SvQml", 1, 0, "SvQmlJsonFile" );
  qmlRegisterType<SvQmlJsonString>          ("SvQml", 1, 0, "SvQmlJsonString" );
  qmlRegisterType<SvQmlJsonHistory>         ("SvQml", 1, 0, "SvQmlJsonHistory" );
  qmlRegisterType<SvQmlJsonModel>           ("SvQml", 1, 0, "SvQmlJsonModel" );
  qmlRegisterType<QmlMidiTrack>             ("SvQml", 1, 0, "QmlMidiTrack" );


  QQmlApplicationEngine engine;

  //Help provider
  SvQmlHtmlImageProvider *guideImageProvider = new SvQmlHtmlImageProvider();
  engine.addImageProvider( "guideImageProvider", guideImageProvider );
  engine.rootContext()->setContextProperty("guideImageProvider", guideImageProvider);

  engine.rootContext()->setContextProperty( "version", QVariant(VERSION) );


  //=============================================================================
  //        Audio output setup
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

  //...and start audio device
  //At this point audio device will be output audio stream to the standard
  //audio output
  audio->start( device );



  //=============================================================================
  //        MIDI synthesator setup
  //Start midi synthesator
  SfSynth *synth = new SfSynth();

  //Synthesator output connect to SoundBufferIODevice
  synth->connect( synth, &SfSynth::noteOn, device, &SoundBufferIODevice::addNote );

  //Inject synth to visual subsystem
  engine.rootContext()->setContextProperty( "synth", synth );



  //=============================================================================
  //        MIDI keyboard setup
  //Start midi keyboard
  QThread *midiThread = new QThread();
  MidiInput *midiInput = new MidiInput( midiThread );
  //Keyboard reports about their connection and disconnection
  midiInput->connect( midiInput, &MidiInput::connectionChanged, synth, &SfSynth::midiConnection );



  //=============================================================================
  //        MIDI processor setup
  MidiProcessor *midiProcessor = new MidiProcessor( midiThread );
  //From midi keyborad output connect to midi processor input
  midiProcessor->connect( midiInput, &MidiInput::midiSignal, midiProcessor, &MidiProcessor::midiKeyboard );
  //From midi processor output connect to synthesator input
  midiProcessor->connect( midiProcessor, &MidiProcessor::midiSignal, synth, &SfSynth::midiSlot );
  midiProcessor->connect( midiProcessor, &MidiProcessor::voiceSetup, synth, &SfSynth::channelSetVoiceId );

  //Inject midi processor to visual subsystem
  //engine.rootContext()->setContextProperty( "midiProcessor", midiProcessor );
  engine.rootContext()->setContextProperty( "qmlKeyboard", midiProcessor->qmlKeyboard() );
  engine.rootContext()->setContextProperty( "qmlMidiFile", midiProcessor->qmlMidiFile() );
  engine.rootContext()->setContextProperty( "qmlMidiFileTrackModel", midiProcessor->qmlMidiFile()->qmlMidiTrackModel() );
  engine.rootContext()->setContextProperty( "qmlStyleFile", midiProcessor->qmlStyleFile() );
  engine.rootContext()->setContextProperty( "qmlStyleFileTrackModel", midiProcessor->qmlStyleFile()->qmlMidiTrackModel() );

  //Inject all midi tracks to visual subsystem
  for( int i = 0; i < 16; i++ )
    engine.rootContext()->setContextProperty( QString("qmlMidiTrack%1Model").arg(i), midiProcessor->qmlMidiFile()->qmlMidiTrack(i) );

  //=============================================================================
  //        MIDI file setup



  midiThread->start();


  //=============================================================================
  //        At the end we start qml application
  const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
  engine.load(url);

//  MidiFile midi;
  //midi.read( "/home/salilab/midi/white_dove.mid" );
  //midiProcessor->qmlMidiFile()->read("/home/salilab/midi/nem.mid");

  return app.exec();
  }
