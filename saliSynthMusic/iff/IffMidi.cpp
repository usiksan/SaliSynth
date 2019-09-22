#include "IffMidi.h"

IffMidi::IffMidi()
  {

  }

bool IffMidi::read(QString fname)
  {
  IffReader reader(fname, false);
  if( !reader.isValid() )
    return false;

  //Header
  if( !reader.readChunk( "MThd", [this] (IffReader &reader) -> bool { return readMthd(reader); }) )
    return false;

  //Repeated tracks
  while( !reader.isEnd() ) {
    IffReader track = reader.getChunk();
    if( track.compareChunkName("MTrk") )
      readMtrk( track );
    }
  return true;
  }

bool IffMidi::readMthd(IffReader &reader)
  {

  }

void IffMidi::readMtrk(IffReader &reader)
  {

  }
