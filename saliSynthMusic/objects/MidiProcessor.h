#ifndef MIDIPROCESSOR_H
#define MIDIPROCESSOR_H

#include <QObject>
#include <QThread>


class MidiProcessor: public QObject
  {
    Q_OBJECT
  public:
    MidiProcessor(  QThread *th, QObject *parent = nullptr);

    //!
    //! \brief emitMidi Emits signal with one midi command. Used
    //! \param cmd      MIDI command
    //! \param data0    MIDI command data 0
    //! \param data1    MIDI command data 1
    //!
    void midiEmit( quint8 cmd, quint8 data0, quint8 data1 );

  signals:
    //!
    //! \brief midi  MIDI export. This signal emits one midi command.
    //!              This signal connected to the wave synth and generate real sound
    //! \param cmd   MIDI command
    //! \param data0 MIDI command data 0
    //! \param data1 MIDI command data 1
    //!
    void midiSignal( quint8 cmd, quint8 data0, quint8 data1 );

  public slots:
    void onStart();

    void midiSlot( quint8 cmd, quint8 data0, quint8 data1 );

  private slots:
    void onTimer();
  };

#endif // MIDIPROCESSOR_H
