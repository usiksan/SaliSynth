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
    SvToolButton {
      icon.source: "qrc:/img/settings.png"
      ToolTip.text: qsTr("Settings")

      onClicked: currentMode = 1
    }

    //Midi file open
    SvToolButton {
      icon.source: "qrc:/img/fileOpen.png"
      ToolTip.text: qsTr("Midi file open")

      onClicked: fileDialogBox.openLoad( qsTr("Open midi file"), synth.midiPath(), ["*.mid","*.kar","*.sty"], function (fname) {
        qmlMidiFile.read( fname );
        //Setup tempo as in midi file
        spinTempo.value = qmlMidiFile.tempo;
      } );
    }

    //Midi config file save
    SvToolButton {
      icon.source: "qrc:/img/fileSave.png"
      ToolTip.text: qsTr("Midi config file save")

      onClicked: qmlMidiFile.configWrite()
    }

    //Stop music
    SvToolButton {
      icon.source: "qrc:/img/stop.png"
      ToolTip.text: qsTr("Stop midi file play")

      onClicked: qmlMidiFile.stop()
    }

    //Play music
    SvToolButton {
      icon.source: "qrc:/img/play.png"
      ToolTip.text: qsTr("Midi file play")

      onClicked: qmlMidiFile.play()
    }

    //Pause music
    SvToolButton {
      icon.source: "qrc:/img/pause.png"
      ToolTip.text: qsTr("Midi file play")

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

      Binding { target: qmlMidiFile; property: "tempo"; value: spinTempo.value }
    }


    //Change view
    SvToolButton {
      icon.source: "qrc:/img/play-music.png"
      ToolTip.text: qsTr("Midi track view")

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
      ifNoTrackFound:  qsTr("No midi file loaded")
    }

    MidiTrackVertical {
      id: midiViewVertical
      visible: midiViewMode === 1
    }
  }
}
