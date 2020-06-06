/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Common checkable tool button
*/
import QtQuick 2.8
import QtQuick.Controls 2.5

ToolButton {
  id: but1
  property bool checkSet : false
  //checkable: true
  icon.color: "transparent"
  ToolTip.visible: hovered
  ToolTip.delay: 300
  background: Rectangle {
    implicitHeight: 26
    implicitWidth: 26
    color: "transparent"
    border.color: "green"
    border.width: but1.checkSet ? 2 : 0
  }
}
