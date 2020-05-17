/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Display accompaniment mode
*/
import QtQuick 2.5
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
      ToolTip.text: qsTr("Style file open")

      onClicked: fileDialogBox.openLoad( qsTr("Open style file"), synth.stylePath(), ["*.sty"], function (fname) {
        qmlStyleFile.read( fname );
        //Setup tempo as in style file
        spinTempo.value = qmlStyleFile.tempo;
      } );
    }

    //Midi config file save
    SvToolButton {
      icon.source: "qrc:/img/fileSave.png"
      ToolTip.text: qsTr("Style config file save")

      onClicked: qmlStyleFile.configWrite()
    }


    //Stop music
    SvToolButton {
      icon.source: "qrc:/img/stop.png"
      ToolTip.text: qsTr("Stop style file play")

      onClicked: qmlStyleFile.stop()
    }


    //Tempo
    SpinBox {
      id: spinTempo
      from: 1
      to: 512
      value: 16
      ToolTip.text: qsTr("Style play tempo")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      Binding { target: qmlStyleFile; property: "tempo"; value: spinTempo.value }
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
      anchors.left: parent.left
      anchors.top: parent.top
      anchors.right: parent.right
      anchors.bottom: partRow.top
      trackModel: qmlStyleFileTrackModel
      ifNoTrackFound:  qsTr("No style file loaded")

      voiceInfoHeight: utils.getFMin( height / 3 - 8, 100 );
    }

    Row {
      id: partRow
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      anchors.right: parent.right
      spacing: 1

      PartButton {
        text: qsTr("Intro A")
        mask: 0x0001
        toolTipText: qsTr("Start play Intro A")
      }
      PartButton {
        text: qsTr("Intro B")
        mask: 0x0010
        toolTipText: qsTr("Start play Intro B")
      }
      PartButton {
        text: qsTr("Intro C")
        mask: 0x0100
        toolTipText: qsTr("Start play Intro C")
      }
      PartButton {
        text: qsTr("Intro D")
        mask: 0x1000
        toolTipText: qsTr("Start play Intro D")
      }

      PartButton {
        text: qsTr("Main A")
        mask: 0x0002
        isMain: true
        toolTipText: qsTr("Select as current or play Main A or play Fill in A")
      }
      PartButton {
        text: qsTr("Main B")
        mask: 0x0020
        isMain: true
        toolTipText: qsTr("Select as current or play Main B or play Fill in B")
      }
      PartButton {
        text: qsTr("Main C")
        mask: 0x0200
        isMain: true
        toolTipText: qsTr("Select as current or play Main C or play Fill in C")
      }
      PartButton {
        text: qsTr("Main D")
        mask: 0x2000
        isMain: true
        toolTipText: qsTr("Select as current or play Main D or play Fill in D")
      }

      PartButton {
        text: qsTr("End A")
        mask: 0x0004
        toolTipText: qsTr("Play End A and stop")
      }
      PartButton {
        text: qsTr("End B")
        mask: 0x0040
        toolTipText: qsTr("Play End B and stop")
      }
      PartButton {
        text: qsTr("End C")
        mask: 0x0400
        toolTipText: qsTr("Play End C and stop")
      }
      PartButton {
        text: qsTr("End D")
        mask: 0x4000
        toolTipText: qsTr("Play End D and stop")
      }


    }
  }
}
