/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Base background for all modes
*/
import QtQuick 2.8

BorderImage {
  border.left: 100; border.top: 100
  border.right: 100; border.bottom: 100
  anchors.fill: parent
  source: "qrc:/img/backMain.png"

  //Exclude press to backgrounds
  MouseArea {
    anchors.fill: parent
    //Do nothing
  }
}
