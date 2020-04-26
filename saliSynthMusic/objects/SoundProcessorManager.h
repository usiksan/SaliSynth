/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   SoundProcessorManager class
*/
#ifndef SOUNDPROCESSORMANAGER_H
#define SOUNDPROCESSORMANAGER_H

#include <QObject>

class SoundProcessorManager : public QObject
  {
    Q_OBJECT
  public:
    explicit SoundProcessorManager(QObject *parent = nullptr);

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
    void midiSlot( quint8 cmd, quint8 data0, quint8 data1 );
  };

#endif // SOUNDPROCESSORMANAGER_H
