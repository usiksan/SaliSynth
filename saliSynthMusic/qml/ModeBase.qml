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

Rectangle {
  color: "white"
  border.width: 0
  anchors.fill: parent

  //Exclude press to backgrounds
  MouseArea {
    anchors.fill: parent
    //Do nothing
  }
}
