/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Sound font file selection
*/
import QtQuick 2.8
import Qt.labs.folderlistmodel 2.1
import QtQuick.Controls 2.5

ModeBase {
  id: soundFontLoader
  visible: false

  //Title
  Text {
    font.bold: true
    font.pointSize: 16
    text: qsTr("Select sound font to load")
    color: 'white'
    anchors.horizontalCenter: parent.horizontalCenter
    horizontalAlignment: Qt.AlignHCenter
    y: 3
  }

  signal selected( string str );

  function selectFont( currentFont ) {
    soundFontLoader.visible = true;
    //Find index of current font in file list
    var index = files.indexOf( "file://" + synth.soundFontPath() + currentFont + ".sf2" );
    //If file found then make it current
    if( index >= 0 ) {
      view.currentIndex = index;
      //Make current index visible
      view.positionViewAtIndex( index, ListView.Contain );
    }
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

    FolderListModel {
      id: files
      folder: "file://" + synth.soundFontPath();
      nameFilters: [ "*.sf2" ]
      showDirs: false
      sortField: FolderListModel.Name
      sortReversed: false
    }

    Component {
      id: fileDelegate

      Rectangle {
        id: wrapper
        width: list.width - 30
        height: 30
        color: "transparent"

        SvText {
          id: nameText
          anchors.fill: parent; verticalAlignment: Text.AlignVCenter
          text: fileBaseName
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
            soundFontLoader.visible = false;
            selected( nameText.text );
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
        model: files
        delegate: fileDelegate
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
      ToolTip.text: qsTr("Close font selector without any changes")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: soundFontLoader.visible = false
    }

  }

}
