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


  //Keyboard itself
  //White keys row [Ряд белых клавиш]
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

  //Black keys row [Ряд черных клавиш]
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



  //Left keyboard
  Rectangle {
    id: leftSide
    x: 0
    y: parent.whiteKeyHeight
    width: qmlKeyboard.delimiter * qmlKeyboard.whiteKeyWidth
    height: parent.height - y
    border.width: 0
    color: "yellow"

    //Left keyboard mode
    SvText {
      anchors.centerIn: parent
      text: (qmlKeyboard.leftMode & 1 ? qsTr("[Chord mode]") : qsTr("[Key mode]") ) +
            (qmlKeyboard.leftMode & 2 ? qsTr("Accomp style:\n") + qmlStyleFile.midiName  : qsTr("Voice:\n") + synth.leftVoice)
    }

    //Select voice for this part
    MouseArea {
      anchors.fill: parent
      onClicked: voiceSelector.selectVoice( true, qmlKeyboard.leftMode, synth.channelVoiceId(1), function (mode,voiceRow) {
        synth.channelSetVoiceRow( 1, voiceRow );
        qmlKeyboard.leftMode = mode;
      } );
    }

    //Left keyboard main voice volume
    Image {
      anchors.top: parent.top
      anchors.left: parent.left
      height: parent.height / 2
      source: "qrc:/img/sound.png"
      fillMode: Image.PreserveAspectFit

      VolumeRegulator {
        id: leftRegulator
        anchors.left: parent.left
        anchors.bottom: parent.top
        volumeValue: qmlKeyboard.leftVolume
        onDomoved: qmlKeyboard.leftVolume = volume
      }

      MouseArea {
        id: leftVolume
        anchors.fill: parent
        hoverEnabled: true
        onClicked: leftRegulator.visible = !leftRegulator.visible
      }

      ToolTip {
        visible: leftVolume.containsMouse
        delay: 200
        text: qsTr("Left voice volume. Press to change volume")
      }
    }
  }

  //Right keyboard main part
  Rectangle {
    id: rightSideMain
    x: leftSide.width
    y: parent.whiteKeyHeight
    width: qmlKeyboard.keyboardWidth - x
    height: 15
    border.width: 0
    color: "dark green"

    //Right main keyboard mode
    SvText {
      anchors.centerIn: parent
      text: (qmlKeyboard.rightMode & 1 ? qsTr("[Chord mode]") : qsTr("[Key mode]") ) + synth.rightMainVoice
      color: "white"
    }

    //Select voice for this part
    MouseArea {
      anchors.fill: parent
      onClicked: voiceSelector.selectVoice( false, qmlKeyboard.rightMode, synth.channelVoiceId(0), function ( mode, voiceRow ) {
        synth.channelSetVoiceRow( 0, voiceRow );
        qmlKeyboard.rightMode = mode;
      } );
    }

    //Right keyboard main voice volume
    Image {
      anchors.top: parent.top
      anchors.bottom: parent.bottom
      anchors.right: parent.right
      source: "qrc:/img/sound.png"
      fillMode: Image.PreserveAspectFit

      VolumeRegulator {
        id: rightMainRegulator
        anchors.right: parent.right
        anchors.bottom: parent.top
        volumeValue: qmlKeyboard.rightMainVolume
        onDomoved: qmlKeyboard.rightMainVolume = volume
      }

      MouseArea {
        id: rightMainVolume
        anchors.fill: parent
        hoverEnabled: true
        onClicked: rightMainRegulator.visible = !rightMainRegulator.visible
      }

      ToolTip {
        visible: rightMainVolume.containsMouse
        delay: 200
        text: qsTr("Main voice volume. Press to change volume")
      }
    }
  }

  //Right keyboard overlay part
  Rectangle {
    id: rightSideOverlay
    x: rightSideMain.x
    y: rightSideMain.y + rightSideMain.height
    width: rightSideMain.width
    height: rightSideMain.height
    border.width: 0
    color: "dark blue"

    //Right slave keyboard mode
    SvText {
      anchors.centerIn: parent
      text: (qmlKeyboard.rightMode & 2 ? synth.rightSlaveVoice : "---------" )
      color: "white"
    }

    //Select voice for this part
    MouseArea {
      anchors.fill: parent
      onClicked: voiceSelector.selectVoice( false, qmlKeyboard.rightMode, synth.channelVoiceId(2), function ( mode, voiceRow ) {
        synth.channelSetVoiceRow( 2, voiceRow );
        qmlKeyboard.rightMode = mode;
      } );
    }

    //Right keyboard overlay voice volume
    Image {
      anchors.top: parent.top
      anchors.bottom: parent.bottom
      anchors.right: parent.right
      source: "qrc:/img/sound.png"
      fillMode: Image.PreserveAspectFit

      VolumeRegulator {
        id: rightOverlayRegulator
        anchors.right: parent.right
        anchors.bottom: parent.top
        volumeValue: qmlKeyboard.rightOverlayVolume
        onDomoved: qmlKeyboard.rightOverlayVolume = volume
      }

      MouseArea {
        id: rightOverlayVolume
        anchors.fill: parent
        hoverEnabled: true
        onClicked: rightOverlayRegulator.visible = !rightOverlayRegulator.visible
      }

      ToolTip {
        visible: rightOverlayVolume.containsMouse
        delay: 200
        text: qsTr("Overlay voice volume. Press to change volume")
      }
    }
  }

  //Left-right keyboard delimiter
  Image {
    source: "qrc:/img/delimiter.png"
    anchors.horizontalCenter: leftSide.right
    anchors.verticalCenter: leftSide.verticalCenter

    MouseArea {
      id: delimiter
      hoverEnabled: true
      anchors.fill: parent
      onClicked: qmlKeyboard.delimiter = -1;
    }

    ToolTip {
      visible: delimiter.containsMouse
      delay: 200
      text: qsTr("Delimiter. Press to change delimiter position")
    }
  }


}
