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

class MidiFile
  {
    quint16 mFormat; //Midi file format (0 or 1)
    quint16 mTrackNumber;
    quint16 mDivision;
  public:
    MidiFile();

    bool read( QString fname );

  private:
    bool    readMthd( IffReader &reader );
    void    readMtrk( IffReader &reader );
    quint32 variableLenValue( IffReader &reader );
  };

#endif // MIDIFILE_H
