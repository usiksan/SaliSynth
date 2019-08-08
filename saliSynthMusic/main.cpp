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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

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
  SoundBufferIODevice *device = new SoundBufferIODevice();
  device->open( QIODevice::ReadOnly );
  audio->setNotifyInterval(5);
  audio->setBufferSize(4096);

  SoundPolyphonyManager::mSoundPolyphony[10].setPeriod(88);
  SoundPolyphonyManager::mSoundPolyphony[50].setPeriod(130);

  qDebug() << "buffer size" << audio->bufferSize() << "notify interval" << audio->notifyInterval() << " period size" << audio->periodSize()
           << " bytes free" << audio->bytesFree();
  audio->start( device );

  qDebug() << "buffer size" << audio->bufferSize() << "notify interval" << audio->notifyInterval() << " period size" << audio->periodSize()
           << " bytes free" << audio->bytesFree();

  return app.exec();
  }
