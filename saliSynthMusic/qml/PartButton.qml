/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Part button for style parts
*/
import QtQuick 2.5
import QtQuick.Controls 2.5

Button {
  property int    mask                //Part mask
  property bool   isMain      : false //Indicator for main part
  property string toolTipText         //Tool tip text for the button

  width: qmlKeyboard.keyboardWidth / 12 - 1
  enabled: qmlStyleFile.parts & mask
  onClicked: qmlStyleFile.playPart(mask)
  ToolTip.text: toolTipText
  ToolTip.visible: hovered
  ToolTip.delay: 300

  Rectangle {
    anchors.fill: parent
    color: "transparent"
    border.color: "blue"
    border.width: 2
    visible: isMain && qmlStyleFile.mainPart & mask
  }

  Rectangle {
    anchors.fill: parent
    color: "transparent"
    border.color: "green"
    border.width: 2
    visible: qmlStyleFile.curPart & mask
  }
}
