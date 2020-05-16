/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Display midi work mode
*/
import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
  anchors.fill: parent

  //Top menu
  Row {
    id: topMenu
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    height: 45
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

      onClicked: fileDialogBox.openLoad( qsTr("Open midi file"), synth.midiPath(), ["*.mid","*.kar","*.sty"], function (fname) {
        qmlMidiFile.read( fname );
        //Setup tempo as in midi file
        spinTempo.value = qmlMidiFile.tickStep;
      } );
    }

    //Midi config file save
    ToolButton {
      icon.source: "qrc:/img/fileSave.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Midi config file save")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: qmlMidiFile.configWrite()
    }

    //Stop music
    ToolButton {
      icon.source: "qrc:/img/stop.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Stop midi file play")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: qmlMidiFile.stop()
    }

    //Play music
    ToolButton {
      icon.source: "qrc:/img/play.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Midi file play")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: qmlMidiFile.play()
    }

    //Pause music
    ToolButton {
      icon.source: "qrc:/img/pause.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Midi file play")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: qmlMidiFile.pause()
    }

    //Tempo
    SpinBox {
      id: spinTempo
      from: 1
      to: 512
      value: 16
      ToolTip.text: qsTr("Midi play tempo")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      Binding { target: qmlMidiFile; property: "tickStep"; value: spinTempo.value }
    }


    //Change view
    ToolButton {
      icon.source: "qrc:/img/play-music.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Midi track view")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: midiViewMode = (midiViewMode + 1) & 1
    }
  }

  property int midiViewMode : 0

  //Work area
  Item {
    anchors.top: topMenu.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    clip: true

    MidiFileSetup {
      id: midiFileSetup
      visible: midiViewMode === 0
    }

    MidiTrackVertical {
      id: midiViewVertical
      visible: midiViewMode === 1
    }
  }
}