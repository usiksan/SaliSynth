import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3


Item {
  anchors.fill: parent

  //Top menu
  Row {
    id: topMenu
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    height: 35
    spacing: 5

    //Settings
    ToolButton {
      icon.source: "qrc:/img/settings.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Settings")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: currentMode = 1
    }

    //Midi file open
    ToolButton {
      icon.source: "qrc:/img/fileOpen.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Midi file open")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: fileDialogBox.openLoad( qsTr("Open midi file"), synth.midiPath(), ["*.mid","*.kar"], null );
    }

  }

  //Work area
  Item {
    anchors.top: topMenu.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: pianoKeyboard.top
    clip: true

    MidiFileSetup {
      id: midiFileSetup
    }
  }


  PianoKeyboard {
    id: pianoKeyboard
    anchors.left: parent.left
    anchors.leftMargin: 3
    anchors.right: parent.right
    anchors.bottom: parent.bottom
  }

  VoiceSelect {
    id: voiceSelector
    anchors.fill: parent
    anchors.bottomMargin: pianoKeyboard.height
  }
}
