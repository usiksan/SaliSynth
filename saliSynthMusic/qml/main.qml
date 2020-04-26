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

  //Sound font map
  SvQmlJsonModel { id: soundFontMap; file: settings; jsonName: "soundFontMap"; fields: [
      "iconName",        //visual icon name for instrument
      "instrumentTitle", //instrument visual title
      "soundFontFile",   //Sound font file name
      "preset",          //Preset in sound font file
      "presetName",      //Preset name in sound font file
      "programm"         //Programm which associates with this preset and sound font file
     ] }

  Binding { target: synth;    property: "model";         value: soundFontMap }


  //Current mode
  property int currentMode : 2
  ModeIntro { visible: currentMode === 0 }
  ModeSetup { visible: currentMode === 1 }
  ModeWork  { visible: currentMode === 2 }

  //Instrument icon selector
  InstrumentIconSelect { id: instrumentIconSelector }

  //Digital input pad
  SvNumPad { id: numPad }

  //Full keyboard
  SvKeyboard { id: keyboard }

  //Pop-up messages
  SvMessageBox { id: messageBox }

  //Query box
  SvQueryBox { id: queryBox }

}
