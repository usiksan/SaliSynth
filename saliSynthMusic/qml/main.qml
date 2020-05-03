/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Visual user interface subsystem
*/
import QtQuick 2.0
import QtQuick.Window 2.0
import SvQml 1.0

Window {
  visible: true
  width: 800
  height: 480
  title: qsTr("Sali Synth Music V") + version

  //Guide path
  property string guidePath: utils.applicationPath + "guide"

  //Utilites
  SvQmlUtils { id: utils }


  //Visual style
  SvStyle { id: svStyle }

  //Settings
  SvQmlJsonFile { id: settings; file: "settings.cfg"; path: utils.homePath + "saliSynthMusic/"; autoSave: 1000 }

  //Voice map
  //It associate single preset from single sound font to cell of voice
  //Cell of voice consist from bank (MSB and LSB) and program number inside bank
  SvQmlJsonModel { id: voiceList; file: settings; jsonName: "voiceList"; fields: [
      //Preset from sound font
      "voiceIconName",            //visual icon name for instrument
      "voiceSoundFontFile",       //Sound font file name
      "voiceSoundFontPreset",     //Preset in sound font file
      "voiceSoundFontPresetName", //Preset name in sound font file
      "voiceVolume",              //Default voice volume to perform normalization

      //The voice that the preset is associated with
      "voiceName",                //Arbitrary voice name
      "voiceBankMsb",             //Voice bank MSB
      "voiceBankLsb",             //Voice bank LSB
      "voiceProgram",             //Voice bank midi program
      "voiceId"                   //Voice id is concatenation of [bank msb][bank lsb][program]
     ] }

  //Channels associations
  SvQmlJsonModel { id: channelList; file: settings; jsonName: "channelList"; fields: [
      "channelVoiceId"
    ] }


  Binding { target: synth;       property: "voiceList";     value: voiceList }
  Binding { target: synth;       property: "channelList";   value: channelList }
  Binding { target: qmlKeyboard; property: "keyboardWidth"; value: width }
  Binding { target: qmlKeyboard; property: "settings";      value: settings }


  //Current mode
  property int currentMode : 2
  ModeIntro { visible: currentMode === 0 }
  ModeSetup { visible: currentMode === 1 }
  ModeWork  { visible: currentMode === 2 }

  //Instrument icon selector
  InstrumentIconSelect { id: instrumentIconSelector }

  //File open dialog
  SvFileDialogBox { id: fileDialogBox }

  //Digital input pad
  SvNumPad { id: numPad }

  //Full keyboard
  SvKeyboard { id: keyboard }

  //Pop-up messages
  SvMessageBox { id: messageBox }

  //Query box
  SvQueryBox { id: queryBox }

}
