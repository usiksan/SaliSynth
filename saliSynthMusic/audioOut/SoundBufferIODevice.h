/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Sound buffer device - interface device with sound card of computer.
   Main idea: sound synthesizes by notes. Simultaneously supported a lot of notes.
   Each of note synthesizes sound separately and SoundBufferIODevice mixes them into
   single stream. When we need to play some note we append it SoundBufferIODevice
   notes play list. When note end playing we remove it from notes play list.
*/
#ifndef SOUNDBUFFERIODEVICE_H
#define SOUNDBUFFERIODEVICE_H

#include "synthConfig.h"
#include "soundFont/SfSynthNote.h"

#include <QIODevice>

using SfSynthNotePtr = SfSynthNote*;

class SoundBufferIODevice : public QIODevice
  {
    Q_OBJECT

    QList<SfSynthNotePtr> mActiveNotes; //! List of current actived notes
  public:
    SoundBufferIODevice();


  public slots:
    //!
    //! \brief addNote Append note to notes play list
    //! \param notePtr Note to append
    //!
    void addNote( SfSynthNotePtr notePtr );

    // QIODevice interface
  public:
    virtual bool   isSequential() const override;
    virtual qint64 bytesAvailable() const override;

  protected:
    //With this sound card queries next portion af data to play
    virtual qint64 readData(char *data, qint64 maxlen) override;

    //Not used
    virtual qint64 writeData(const char *data, qint64 len) override;
  };

#endif // SOUNDBUFFERIODEVICE_H
