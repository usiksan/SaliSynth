/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Common tool button
*/
import QtQuick 2.14
import QtQuick.Controls 2.5

ToolButton {
  icon.color: "transparent"
  ToolTip.visible: hovered
  ToolTip.delay: 300

  background: Rectangle {
    implicitHeight: 26
    implicitWidth: 26
    color: "transparent"
  }
}
