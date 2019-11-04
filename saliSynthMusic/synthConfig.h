/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
 History
   06.08.2019 v1  Begin sound output test
   04.10.2019 v2  Append midi file reader.
                  Append channel preset changer
*/
#ifndef SYNTHCONFIG_H
#define SYNTHCONFIG_H

//Programm version
#define VERSION 2

//Polyphony count
#define POLYPHONY_COUNT 128

#define SAMPLES_PER_SECOND 48000

#define SAMPLES_PER_20MS   (SAMPLES_PER_SECOND / 50)

#endif // SYNTHCONFIG_H
