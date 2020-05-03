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
    synth.setProgramm( 0, row );
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
        width: parent.width
        height: 30
        Rectangle {
          anchors.fill: parent
          visible: index === currentRow
          color: Qt.lighter( "green" )
        }
        Row {
          spacing: 2
          anchors.verticalCenter: parent.verticalCenter
          //Voice serial number
          SvFieldText {
            text: index + 1
            width: widthNpp
            editable: false
            onLeftButton: setCurrentRow(index);
          }
          //Voice icon
          Image {
            width: widthIcon
            height: 32
            source: voiceIconName
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
          }
          //Bank LSB
          SvFieldText {
            text: voiceBankLsb
            width: widthId
            editable: false
            onLeftButton: setCurrentRow(index);
          }
          //Midi programm
          SvFieldText {
            text: voiceProgram
            width: widthId
            editable: false
            onLeftButton: setCurrentRow(index);
          }
          //Programm title
          SvFieldText {
            text: voiceName
            width: widthTitle
            editable: false
            onLeftButton: setCurrentRow(index);
          }
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
    ToolButton {
      visible: keyboardLeft
      checkable: true
      checked: currentMode & 2
      icon.source: "qrc:/img/acompaniment.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Accompaniment mode")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: currentMode |= 2;
    }
    ToolButton {
      visible: keyboardLeft
      checkable: true
      checked: (currentMode & 2) == 0
      icon.source: "qrc:/img/voice.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Voice mode")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: currentMode &= ~2;
    }

    //Selection simple key or chord mode
    ToolButton {
      visible: keyboardLeft
      checkable: true
      checked: currentMode & 1
      icon.source: "qrc:/img/chord.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Chord mode")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: currentMode |= 1;
    }
    ToolButton {
      visible: keyboardLeft
      checkable: true
      checked: (currentMode & 1) == 0
      icon.source: "qrc:/img/singleKey.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Simple key mode")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: currentMode &= ~1;
    }


    //Right keyboard modes
    ToolButton {
      visible: keyboardRight
      checkable: true
      checked: currentMode & 2
      icon.source: "qrc:/img/doubleVoice.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Double voice mode")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: currentMode |= 2;
    }
    ToolButton {
      visible: keyboardRight
      checkable: true
      checked: (currentMode & 2) == 0
      icon.source: "qrc:/img/voice.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Single voice mode")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: currentMode &= ~2;
    }


    //Apply current selected voice
    ToolButton {
      icon.source: "qrc:/img/yes.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Apply current selected voice")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: {
        if( voiceApplyFunction !== null )
          voiceApplyFunction( currentMode, currentRow );
        voiceSelector.visible = false
      }
    }

    //Close selector
    ToolButton {
      icon.source: "qrc:/img/close.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Close voice selector without any changes")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: voiceSelector.visible = false
    }

  }


}
