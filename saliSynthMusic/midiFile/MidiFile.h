/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   MIDI file
*/
#ifndef MIDIFILE_H
#define MIDIFILE_H

#include "iff/IffReader.h"

#include <QList>


struct MidiEvent
  {
    quint32 mTime = 0;
    quint8  mType = 0;
    quint8  mData0 = 0;
    quint8  mData1 = 0;
  };

using MidiEventList = QList<MidiEvent>;


struct MidiTrack
  {
    QString       mName;
    MidiEventList mEvents;
    quint32       mEventIndex;
  };


using MidiTrackVector = QVector<MidiTrack>;


class MidiFile : public QObject
  {
    Q_OBJECT

    quint16       mFormat;      //Midi file format (0 or 1)
    quint16       mTrackNumber;
    quint16       mDivision;
    quint32         mTempo;

    MidiTrackVector mTracks;      //Tracks in midi file
  public:
    MidiFile();

    bool read( QString fname );

  signals:
    void midiEvent( quint8 cmd, quint8 data0, quint8 data1 );

  public slots:
    void tick();

    void seek( quint32 time );

  private:
    bool    readMthd( IffReader &reader );
    void    readMtrk(IffReader &reader);
    quint32 variableLenValue( IffReader &reader );
  };

#endif // MIDIFILE_H
