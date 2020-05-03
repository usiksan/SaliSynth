/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Instrument icon selector
*/
import QtQuick 2.8
import QtQuick.Controls 2.5

ModeBase {
  id: instrumentIconSelector

  visible: false

  property var selectFun

  property string selectedIcon

  property string hoveredNozzle

  //Icon selection function
  function instrumentIconSelect( curIcon, sfun ) {
    selectedIcon = curIcon;
    selectFun = sfun;
    instrumentIconSelector.visible = true;
  }

  //Available icon list model
  InstrumentIconListModel {
    id: iconListModel
  }


  //Display icons
  Component {
    id: iconDelegate

    Item {
      id: wrapper
      width: 120
      height: 80

      Image {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        source: iconSrc
      }

      SvText {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        text: iconTitle
      }

      //Current icon marker
      Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "green"
        border.width: 3
        visible: iconSrc === selectedIcon
      }

      MouseArea {
        id: mouseRegion
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
          instrumentIconSelector.visible = false;
          if( mouse.button === Qt.LeftButton && selectFun !== null ) {
            selectFun( iconSrc );
          }
        }
      }
    }
  }

  SvScrollView {
    anchors.fill: parent

    GridView {
      id: view
      anchors.fill: parent
      anchors.margins: 10
      cellWidth: 122
      cellHeight: 82
      model: iconListModel
      delegate: iconDelegate
      focus: true
      clip: true
    }
  }

  //Close selector
  ToolButton {
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.margins: 5
    icon.source: "qrc:/img/close.png"
    icon.color: "transparent"
    ToolTip.text: qsTr("Close instrument icon selector without any changes")
    ToolTip.visible: hovered
    ToolTip.delay: 300

    onClicked: instrumentIconSelector.visible = false
  }


}
