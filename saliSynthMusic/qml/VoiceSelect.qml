/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Display preset names list
   Allow to select single preset from list
*/
import QtQuick 2.8
import QtQuick.Controls 2.5
import SvQml 1.0

Rectangle {
  id: voiceSelector
  border.width: 0
  color: "white"
//  anchors.fill: parent

  visible: false

  property bool keyboardLeft
  property bool keyboardRight

  property int widthNpp: 50
  property int widthId: 80
  property int widthIcon: 64
  property int widthTitle: 150

  property int currentRow : 0
  property int currentMode : 0
  property var voiceApplyFunction : null

  function selectVoice( isLeft, mode, voiceId, applyFunction ) {
    keyboardLeft = isLeft
    keyboardRight = !isLeft
    currentMode = mode
    voiceSelector.visible = true
    currentRow = synth.voiceRowById(voiceId)
    voiceApplyFunction = applyFunction
  }

  function selectVoiceEx( voiceId, applyFunction ) {
    keyboardLeft = false
    keyboardRight = false
    currentMode = 0
    voiceSelector.visible = true
    currentRow = synth.voiceRowById(voiceId)
    voiceApplyFunction = applyFunction
  }

  //Exclude press on z-bottom elements
  MouseArea {
    anchors.fill: parent
  }

  //Title
  Text {
    font.bold: true
    font.pointSize: 16
    text: qsTr("Select voice")
    color: 'black'
    anchors.horizontalCenter: parent.horizontalCenter
    horizontalAlignment: Qt.AlignHCenter
    y: 3
  }

  function setCurrentRow( row ) {
    currentRow = row;
  }


   //Table title
  Row {
    id: idTitle
    anchors.top: parent.top
    anchors.topMargin: 35
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.right: parent.right
    anchors.rightMargin: 5
    height: 25

    spacing: 2
    //Programm number
    SvTableHeader {
      text: qsTr("Nprog")
      width: widthNpp
    }
    //Instrument icon
    SvTableHeader {
      //text: qsTr("")
      width: widthIcon
    }
    SvTableHeader {
      text: qsTr("Bank MSB")
      width: widthId
    }
    SvTableHeader {
      text: qsTr("Bank LSB")
      width: widthId
    }
    SvTableHeader {
      text: qsTr("Programm")
      width: widthId
    }
    //Programm title
    SvTableHeader {
      text: qsTr("Title")
      width: widthTitle
    }
  }

  //Preset from sound font
//  "voiceIconName",            //visual icon name for instrument
//  "voiceInstrumentTitle",     //instrument visual title
//  "voiceSoundFontFile",       //Sound font file name
//  "voiceSoundFontPreset",     //Preset in sound font file
//  "voiceSoundFontPresetName", //Preset name in sound font file

//  //The voice that the preset is associated with
//  "voiceName",                //Arbitrary voice name
//  "voiceBankMsb",             //Voice bank MSB
//  "voiceBankLsb",             //Voice bank LSB
//  "voiceProgram",             //Voice bank midi program
//  "voiceId"                   //Voice id is concatenation of [bank msb][bank lsb][program]
  //Table contents
  SvScrollView {
    anchors.top: idTitle.bottom
    anchors.topMargin: 5
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.right: parent.right
    anchors.rightMargin: 5
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 40

    clip: true

    ListView {
      anchors.fill: parent

      model: voiceList

      delegate: Item {
        id: rowDelegate
        property bool isCurrent : index === currentRow

        width: parent.width
        height: isCurrent ? 36 : 32

        //Part title
        Item {
          visible: voiceBankMsb === "130"
          anchors.fill: parent

          SvFieldText {
            anchors.centerIn: parent
            text: voiceName
          }
        }

        //Voice
        Row {
          visible: voiceBankMsb !== "130"
          spacing: 2
          anchors.verticalCenter: parent.verticalCenter
          //Voice serial number
          SvFieldText {
            text: index + 1
            width: widthNpp
            horizontalAlignment: Text.AlignHCenter
            editable: false
            onLeftButton: setCurrentRow(index);
            anchors.verticalCenter: parent.verticalCenter
          }
          //Voice icon
          Rectangle {
            width: widthIcon
            height: 32
            color: "white"
            Image {
              source: voiceIconName
              anchors.fill: parent
              fillMode: Image.PreserveAspectFit
            }
            MouseArea {
              anchors.fill: parent
              acceptedButtons: Qt.LeftButton | Qt.RightButton
              onClicked: setCurrentRow(index);
            }
          }

          //Bank MSB
          SvFieldText {
            text: voiceBankMsb
            width: widthId
            editable: false
            onLeftButton: setCurrentRow(index);
            anchors.verticalCenter: parent.verticalCenter
          }
          //Bank LSB
          SvFieldText {
            text: voiceBankLsb
            width: widthId
            editable: false
            onLeftButton: setCurrentRow(index);
            anchors.verticalCenter: parent.verticalCenter
          }
          //Midi programm
          SvFieldText {
            text: voiceProgram
            width: widthId
            editable: false
            onLeftButton: setCurrentRow(index);
            anchors.verticalCenter: parent.verticalCenter
          }
          //Programm title
          SvFieldText {
            text: voiceName
            width: widthTitle
            editable: false
            onLeftButton: setCurrentRow(index);
            anchors.verticalCenter: parent.verticalCenter
          }
        }
        //Selecting rectangle
        Rectangle {
          anchors.fill: parent
          visible: rowDelegate.isCurrent
          color: "transparent"
          border.color: Qt.lighter( "green" )
          border.width: 2
        }
      }
    }


  }


  //Local menu
  Row {
    anchors.top: parent.top
    anchors.right: parent.right
    height: 24
    spacing: 5

    //Left keyboard mode buttons
    //Selection acompaniment or simple voice mode
    SvCheckToolButton {
      visible: keyboardLeft
      checkSet: (currentMode & QmlKeyboard.KmLeftMask) == QmlKeyboard.KmLeftAccomp
      icon.source: "qrc:/img/acompaniment.png"
      ToolTip.text: qsTr("Accompaniment mode")

      onClicked: currentMode = (currentMode & ~QmlKeyboard.KmLeftMask) | QmlKeyboard.KmLeftAccomp;
    }

    SvCheckToolButton {
      visible: keyboardLeft
      checkSet: (currentMode & QmlKeyboard.KmLeftMask) == QmlKeyboard.KmLeftVoice
      icon.source: "qrc:/img/voice.png"
      ToolTip.text: qsTr("Voice mode")

      onClicked: currentMode = (currentMode & ~QmlKeyboard.KmLeftMask) | QmlKeyboard.KmLeftVoice;
    }

    //Selection simple key or chord mode
    SvCheckToolButton {
      visible: keyboardLeft
      checkSet: (currentMode & QmlKeyboard.KmChordMask) == QmlKeyboard.KmChordActive
      icon.source: "qrc:/img/chord.png"
      ToolTip.text: qsTr("Chord mode")

      onClicked: currentMode = (currentMode & ~QmlKeyboard.KmChordMask) | QmlKeyboard.KmChordActive;
    }

    SvCheckToolButton {
      visible: keyboardLeft
      checkSet: (currentMode & QmlKeyboard.KmChordMask) == QmlKeyboard.KmChordPassive
      icon.source: "qrc:/img/singleKey.png"
      ToolTip.text: qsTr("Simple key mode")

      onClicked: currentMode = (currentMode & ~QmlKeyboard.KmChordMask) | QmlKeyboard.KmChordPassive;
    }


    //Right keyboard modes
    SvCheckToolButton {
      visible: keyboardRight
      checkSet: (currentMode & QmlKeyboard.KmRightMask) == QmlKeyboard.KmRightOverlay
      icon.source: "qrc:/img/doubleVoice.png"
      ToolTip.text: qsTr("Double voice mode")

      onClicked: currentMode = (currentMode & ~QmlKeyboard.KmRightMask) | QmlKeyboard.KmRightOverlay;
    }
    SvCheckToolButton {
      visible: keyboardRight
      checkSet: (currentMode & QmlKeyboard.KmRightMask) == QmlKeyboard.KmRightSingle
      icon.source: "qrc:/img/voice.png"
      ToolTip.text: qsTr("Single voice mode")

      onClicked: currentMode = (currentMode & ~QmlKeyboard.KmRightMask) | QmlKeyboard.KmRightSingle;
    }


    //Apply current selected voice
    SvToolButton {
      icon.source: "qrc:/img/yes.png"
      ToolTip.text: qsTr("Apply current selected voice")

      onClicked: {
        if( voiceApplyFunction !== null )
          voiceApplyFunction( currentMode, currentRow );
        voiceSelector.visible = false
      }
    }

    //Close selector
    SvToolButton {
      icon.source: "qrc:/img/close.png"
      ToolTip.text: qsTr("Close voice selector without any changes")

      onClicked: {
        //Clear apply function
        voiceApplyFunction = null;
        voiceSelector.visible = false;
      }
    }

  }


}
