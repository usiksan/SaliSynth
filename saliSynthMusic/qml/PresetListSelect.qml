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

ModeBase {
  id: presetSelector
  visible: false

  //Title
  Text {
    font.bold: true
    font.pointSize: 16
    text: qsTr("Select preset to assign to current programm")
    color: 'black'
    anchors.horizontalCenter: parent.horizontalCenter
    horizontalAlignment: Qt.AlignHCenter
    y: 3
  }

  //Calling when user selected preset
  signal selected( int presetIndex );



  //This function start preset selection procedure
  // it calls from outside to select preset from available presets in current programm sound font
  //currProg - current programm index
  //currPreset - current preset index
  function presetSelect( currProg, currPreset ) {
    //On current programm we load preset list model
    view.model = synth.presetList( currProg );
    //..select current preset
    view.currentIndex = currPreset;
    //..and show this form
    presetSelector.visible = true;
  }




  //Sound font list
  Rectangle {
    id: list
    anchors.fill: parent
    anchors.leftMargin: 10
    anchors.topMargin: 50
    anchors.rightMargin: 10
    anchors.bottomMargin: 5
    border.color: svStyle.frontColor
    color: "transparent"

    Component {
      id: presetDelegate

      Rectangle {
        id: wrapper
        width: list.width - 30
        height: 30
        color: "transparent"

        SvText {
          id: presetText
          anchors.fill: parent; verticalAlignment: Text.AlignVCenter
          text: modelData
          anchors.leftMargin: 10
          font.pixelSize: 13
          elide: Text.ElideRight
          wrapMode: Text.WordWrap
          color: svStyle.frontColor
        }

        MouseArea {
          id: mouseRegion
          anchors.fill: parent
          onClicked: {
            presetSelector.visible = false;
            selected( index );
            wrapper.ListView.view.currentIndex = index;
            }
        }
      }
    }

    SvScrollView {
      anchors.fill: parent

      ListView {
        id: view
        anchors.fill: parent
        anchors.margins: 10
        //model: files
        delegate: presetDelegate
        focus: true
        highlight: Rectangle {
          color: svStyle.buttonBackDefault
          width: view.width - 10
          border.color: svStyle.frontColor
        }
        highlightMoveVelocity: 1000
        pressDelay: 100
        clip: true
      }

    }
  }

  //Local menu
  Row {
    anchors.top: parent.top
    anchors.right: parent.right
    height: 24
    spacing: 5

    //Apply current font
//    ToolButton {
//      icon.source: "qrc:/img/yes.png"
//      icon.color: "transparent"
//      ToolTip.text: qsTr("Apply current font")
//      ToolTip.visible: hovered
//      ToolTip.delay: 300

//      onClicked: synth.voiceAdd();
//    }

    //Close selector
    ToolButton {
      icon.source: "qrc:/img/close.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Close preset selector without any changes")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: presetSelector.visible = false
    }

  }

}
