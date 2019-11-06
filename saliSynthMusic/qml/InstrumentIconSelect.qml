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
      width: 64
      height: 64

      Image {
        anchors.centerIn: parent
        source: iconSrc
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
      cellWidth: 64
      cellHeight: 64
      model: iconListModel
      delegate: iconDelegate
      focus: true
      clip: true
    }
  }

}
