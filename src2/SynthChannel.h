/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SYNTHCHANNEL_H
#define SYNTHCHANNEL_H

#define BUFFER_SIZE 441

class SynthChannel
  {
    int mBuffer[BUFFER_SIZE * 2]; //Channel buffer
  public:
    SynthChannel();

    //Midi
    void noteOn( int noteIndex, int dynamic );
    void noteOff( int noteIndex, int dynamic );
    void polyphonicKey( int noteIndex, int dynamic );
    void controlChange( int controllerIndex, int controllerValue );
    void programmChange( int programmIndex );
    void channelPressure( int pressure );
    void pichWheelChange( int lsb, int msb );

    int *prepare();
  };

#endif // SYNTHCHANNEL_H
