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
  width: 640
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
      "iconName",      //visual icon name for instrument
      "title",         //instrument visual title
      "soundFontFile", //Sound font file name
      "preset",        //Preset in sound font file
      "programm"       //Programm which associates with this preset and sound font file
     ] }


  //Background image
  Image {
    anchors.fill: parent
    source: "qrc:/img/backMain.png"
  }

  //Current mode
  property int currentMode : 0
  ModeIntro { visible: currentMode === 0 }
  ModeSetup { visible: currentMode === 1 }

}
