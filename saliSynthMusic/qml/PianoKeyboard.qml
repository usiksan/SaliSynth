/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Display keyboard with press visualisation and mouse operations
*/
import QtQuick 2.8
import QtQuick.Controls 2.5

Item {
  height: qmlKeyboard.whiteKeyWidth * 4
  property int whiteKeyHeight : height - 30



  //Keyboard delimiter
  Rectangle {
    id: leftSide
    x: 0
    y: parent.whiteKeyHeight
    width: qmlKeyboard.delimiter * qmlKeyboard.whiteKeyWidth
    height: parent.height - y
    border.width: 0
    color: "yellow"

    //Select voice for this part
    ToolButton {
      anchors.left: parent.left
      anchors.top: parent.top
      anchors.topMargin: 5
      width: 24
      height: 15
      padding: 0
      icon.source: "qrc:/img/select.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Select voice for left part")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: voiceSelector.selectVoice( channelList.asInt( 1, "channelVoiceId" ), null )
    }
  }

  Rectangle {
    id: rightSideMain
    x: leftSide.width
    y: parent.whiteKeyHeight
    width: qmlKeyboard.keyboardWidth - x
    height: 15
    border.width: 0
    color: "dark green"

    SvText {
      id: rightSideMainText
      anchors.centerIn: parent
      text: synth.channelPresetName(0)
      color: "white"

      Connections {
        target: synth
        onChannelPresetChanged: {
          if( channel === 0 ) rightSideMainText.text = presetName;
        }
      }
    }
  }

  Rectangle {
    id: rightSideOverlay
    x: rightSideMain.x
    y: rightSideMain.y + rightSideMain.height
    width: rightSideMain.width
    height: rightSideMain.height
    border.width: 0
    color: "dark blue"
  }

  Image {
    source: "qrc:/img/delimiter.png"
    anchors.horizontalCenter: leftSide.right
    anchors.verticalCenter: leftSide.verticalCenter

    MouseArea {
      anchors.fill: parent
      onClicked: qmlKeyboard.delimiter = -1;
    }
  }

  //Ряд белых клавиш
  Repeater {
    model: qmlKeyboard

    Rectangle {
      visible: !keyIsBlack
      x: keyIndex * qmlKeyboard.whiteKeyWidth
      y: 0
      width: qmlKeyboard.whiteKeyWidth
      height: leftSide.y
      border.width: 1
      border.color: "black"
      color: "white"

      //Key pressed indication
      Rectangle {
        visible: keyColor & 3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        width: 8
        height: 8
        radius: 4
        border.width: 0
        color: "green"
      }

      //Chord indication
      Rectangle {
        visible: keyColor & 4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 14
        width: 8
        height: 8
        radius: 4
        border.width: 0
        color: "red"
      }

      MouseArea {
        anchors.fill: parent
        onPressed: keyColor = true
        onReleased: keyColor = false
      }
    }

  }

  //Ряд черных клавиш
  Repeater {
    model: qmlKeyboard

    Rectangle {
      visible: keyIsBlack
      x: (qmlKeyboard.whiteKeyWidth + width) / 2 + keyIndex * qmlKeyboard.whiteKeyWidth
      y: 0
      width: qmlKeyboard.whiteKeyWidth / 2 + 2
      height: leftSide.y * 3 / 5
      color: "black"

      //Key pressed indication
      Rectangle {
        visible: keyColor & 3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        width: 8
        height: 8
        radius: 4
        border.width: 0
        color: "green"
      }


      //Chord indication
      Rectangle {
        visible: keyColor & 4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 14
        width: 8
        height: 8
        radius: 4
        border.width: 0
        color: "red"
      }

      MouseArea {
        anchors.fill: parent
        onPressed: keyColor = true
        onReleased: keyColor = false
      }
    }
  }

}
