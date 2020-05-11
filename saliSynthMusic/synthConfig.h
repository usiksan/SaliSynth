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
              v7  Append delimiter functionality
              v8  Append visual representation of keyborad on model-view basis
              v9  Append press key visualisation on screen
                  Append possibility to play on keyboard by screen representation press
              v10 Append delimiter function
                  Append delimiter visual ajust with mouse or midi keyboard
                  Make keyboard drawing resizeble
              v11 Append keyboard channel preset name visualisation
              v12 Refactoring preset list. At now it has become an array of objects, not pointers.
                  Also array ob objects become channels.
              v13 Append yamaha simple chord generator
                  Append mode for left and right sides of keyboard
              v14 First time of midi file play
   27.04.2020 v15 MIDI file read error: group events
                  QmlMidiFile at now object and tracks are listModels
   29.04.2020 v16 Refactoring voice list
                  Refactoring qml pages to select sound font, select preset
                  Append qml page to select voice
                  Append some icons
   01.05.2020 v17 Fix error: not edited voice name
                  Append location font selector font to current font
              v18 Append selection voices for all keyboard parts
                  Append packet preset appendion
                  Fix error: deleting voice row
              v19 Append mode selection for all keyboard parts
                  Fix error: right keyboard slave channel conversion
                  Append icons for keyboard mode selection
              v20 Begin parsing style file
              v21 Append save keyboard state to settings
              v22 Restore instrument icon selector
   02.05.2020 v23 Design midi track info viewing through used track model
                  Append changes to the midi file through config file saved with original midi,
                  so need not to save midi while
                  Append selection voice for track
                  Append changes track title and save it to the config file
              v24 Append on-off possibility for sound of the track
              v25 Remove previously MidiTrack structures and replace them by QmlMidiTrack structure that
                  support visual representation
                  Append separate signal for setup voice id for channel
              v26 Append start-stop midi play functionality
              v27 Append plaing tempo changing and store it to the config
                  Try parse track names from markers
              v28 Enhanse release note phase with short release time
              v29 Append a lot of instrument icon
              v30 Append a lot of instrument icon
                  Append title to icon in instrument icon selector
                  Append empty icon
                  Append close icon selector button
              v31 Append volume regulation per channel when midi play
              v32 Begin display midi tracks. Make up vertical preliminary visualization
              v33 Append automatic average exclusion volume ajust
                  Append time moment of play property
              v34 Append time tracking functionality preliminary
              v35 Append all track display model with automatic color selection
              v36 Append track visibility functionality
              v37 Resolve problem with shift to one octave down all drum kits on Yamaha
                  We correct midi track with msb=127 and lsb=0 by shift to one octave up
                  to locate to standard drum kits
   05.05.2020 v38 Begin styled autoacompaniment integration
   06.05.2020 v39 Append visual direction for style play
                  Append fragment looping and queuing, add jump beetween parts
   11.05.2020 v40 Refactoring style part buttons. At now they on bottom row
                  In central part of acompaniment - midi setup with individual track accessing
                  Append style save posibilities
                  Append accord to autoacompaniment transfer
              v41 Append remarks to a lot of files
              v42 Append tool tips to style parts
                  Append lamp icon to midi track info
*/
#ifndef SYNTHCONFIG_H
#define SYNTHCONFIG_H

//Programm version
#define VERSION 42

//Polyphony count
#define POLYPHONY_COUNT 128

#define SAMPLES_PER_SECOND 48000

#define SAMPLES_PER_20MS   (SAMPLES_PER_SECOND / 50)

#endif // SYNTHCONFIG_H
