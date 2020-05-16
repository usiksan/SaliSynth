/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Intro page with work mode selection
*/
import QtQuick 2.8

ModeBase {
  id: introMode


  SvText {
    text: qsTr("Sound fonts loading, please wait...")
    anchors.centerIn: parent
  }

}
