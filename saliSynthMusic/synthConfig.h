/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   This project is software synthesator for hardware midi keyboards.

   A key possibility is SaliSynth can generate synth wave self without any external programm.
   Wave generation is on SoundFont base, so the user can change the sound simply by replacing
     the SoundFont with their own, for example, downloaded from the Internet.

   Main idea is
     [midi keyboard] -> [sound processor set] -> [sound font synthesator] -> [audio output]

     where
     [midi keyboard] - is any midi keyboard with USB connection. This connections automaticaly
                       recognized by Linux
     [sound processor set] - a set of processors used for converting or generating midi commands.
                             For example, a midi file player is a typical processor
     [sound font synthesator] - sound wave synthesator on SoundFont base and midi input
     [audio output] - audio stream to output translator

 History
   06.08.2019 v1  Begin sound output test
   04.11.2019 v2  Append midi file reader.
                  Append channel preset changer
              v3  Creating SfSynth - all programm (font and presets) manager
   06.11.2019 v4  Begin creation visual sound font manager
                  Appended instrument icon selector
                  Appended sound file selector
                  Display name of preset
   07.11.2019 v5  Appended preset selection from sound font preset list
   25.04.2020 v6  Refactoring programm structure. Append MidiProcessor
*/
#ifndef SYNTHCONFIG_H
#define SYNTHCONFIG_H

//Programm version
#define VERSION 6

//Polyphony count
#define POLYPHONY_COUNT 128

#define SAMPLES_PER_SECOND 48000

#define SAMPLES_PER_20MS   (SAMPLES_PER_SECOND / 50)

#endif // SYNTHCONFIG_H
