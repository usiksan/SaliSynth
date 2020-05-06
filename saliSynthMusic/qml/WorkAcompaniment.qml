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

    Grid {
      anchors.centerIn: parent

      columns: 3
      spacing: 5
      //Part A
      Button {
        text: qsTr("Intro A")
        enabled: qmlStyleFile.parts & 0x0001
        onClicked: qmlStyleFile.playPart(0x0001)
      }
      Button {
        text: qsTr("Main A")
        enabled: qmlStyleFile.parts & 0x0002
        onClicked: qmlStyleFile.playPart(0x0002)
      }
      Button {
        text: qsTr("Ending A")
        enabled: qmlStyleFile.parts & 0x0004
        onClicked: qmlStyleFile.playPart(0x0004)
      }

      //Part B
      Button {
        text: qsTr("Intro B")
        enabled: qmlStyleFile.parts & 0x0010
        onClicked: qmlStyleFile.playPart(0x0010)
      }
      Button {
        text: qsTr("Main B")
        enabled: qmlStyleFile.parts & 0x0020
        onClicked: qmlStyleFile.playPart(0x0020)
      }
      Button {
        text: qsTr("Ending B")
        enabled: qmlStyleFile.parts & 0x0040
        onClicked: qmlStyleFile.playPart(0x0040)
      }

      //Part C
      Button {
        text: qsTr("Intro C")
        enabled: qmlStyleFile.parts & 0x0100
        onClicked: qmlStyleFile.playPart(0x0100)
      }
      Button {
        text: qsTr("Main C")
        enabled: qmlStyleFile.parts & 0x0200
        onClicked: qmlStyleFile.playPart(0x0200)
      }
      Button {
        text: qsTr("Ending C")
        enabled: qmlStyleFile.parts & 0x0400
        onClicked: qmlStyleFile.playPart(0x0400)
      }

      //Part D
      Button {
        text: qsTr("Intro D")
        enabled: qmlStyleFile.parts & 0x1000
        onClicked: qmlStyleFile.playPart(0x1000)
      }
      Button {
        text: qsTr("Main D")
        enabled: qmlStyleFile.parts & 0x2000
        onClicked: qmlStyleFile.playPart(0x2000)
      }
      Button {
        text: qsTr("Ending D")
        enabled: qmlStyleFile.parts & 0x4000
        onClicked: qmlStyleFile.playPart(0x4000)
      }
    }
  }
}
