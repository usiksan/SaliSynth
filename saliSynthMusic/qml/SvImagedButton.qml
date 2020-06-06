/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Setup standard button for icon display and no background
*/
import QtQuick 2.0
import QtQuick.Controls 2.5

Button {
  id: but
  property int buttonSize: 68
  property string buttonSource : ""
  property string borderColor : "black"
  property int    borderWidth : buttonImage.source === "" ? 2 : 0

  width: buttonSize
  height: buttonSize
  padding: 0
  contentItem: Image {
    id: buttonImage
    width: but.buttonSize - 4
    height: but.buttonSize - 4
    source: but.buttonSource
    fillMode: Image.PreserveAspectFit
  }
  background: Rectangle {
    width: but.buttonSize
    height: but.buttonSize
    color: "transparent"
    border.color: borderColor
    border.width: borderWidth
  }

  ToolTip.visible: hovered
  ToolTip.delay: 300
}
