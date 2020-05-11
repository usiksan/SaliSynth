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
      ToolTip.text: qsTr("Style file open")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: fileDialogBox.openLoad( qsTr("Open style file"), synth.stylePath(), ["*.sty"], function (fname) {
        qmlStyleFile.read( fname );
        //Setup tempo as in style file
        spinTempo.value = qmlStyleFile.tickStep;
      } );
    }

    //Midi config file save
    ToolButton {
      icon.source: "qrc:/img/fileSave.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Style config file save")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: qmlStyleFile.configWrite()
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


    //Tempo
    SpinBox {
      id: spinTempo
      from: 1
      to: 512
      value: 16
      ToolTip.text: qsTr("Style play tempo")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      Binding { target: qmlStyleFile; property: "tickStep"; value: spinTempo.value }
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

      voiceInfoHeight: utils.getFMin( height / 3 - 3, 100 );
    }

    Row {
      id: partRow
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      anchors.right: parent.right

      PartButton {
        text: qsTr("IntroA")
        mask: 0x0001
      }
      PartButton {
        text: qsTr("IntroB")
        mask: 0x0010
      }
      PartButton {
        text: qsTr("IntroC")
        mask: 0x0100
      }
      PartButton {
        text: qsTr("IntroD")
        mask: 0x1000
      }

      PartButton {
        text: qsTr("MainA")
        mask: 0x0002
        isMain: true
      }
      PartButton {
        text: qsTr("MainB")
        mask: 0x0020
        isMain: true
      }
      PartButton {
        text: qsTr("MainC")
        mask: 0x0200
        isMain: true
      }
      PartButton {
        text: qsTr("MainD")
        mask: 0x2000
        isMain: true
      }

      PartButton {
        text: qsTr("EndA")
        mask: 0x0004
      }
      PartButton {
        text: qsTr("EndB")
        mask: 0x0040
      }
      PartButton {
        text: qsTr("EndC")
        mask: 0x0400
      }
      PartButton {
        text: qsTr("EndD")
        mask: 0x4000
      }


    }
  }
}
