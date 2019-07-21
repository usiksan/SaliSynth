/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QDebug>

#include "Synth.h"
#include "MidiInput.h"

int main(int argc, char *argv[])
  {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  //Audio synthesator
  QThread *audioThread = new QThread();
  Synth   *synth = new Synth( audioThread );
  audioThread->start(QThread::TimeCriticalPriority);

  //Midi usb keyboard reader
  QThread *midiThread = new QThread();
  MidiInput *midiInput = new MidiInput( midiThread );
  midiThread->start();



  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;



  int result = app.exec();

  //Stop midi
  midiThread->quit();
  midiThread->wait();

  delete midiInput;
  delete midiThread;

  //Stop audio
  audioThread->quit();
  audioThread->wait();

  delete synth;
  delete audioThread;

  return result;
  }
