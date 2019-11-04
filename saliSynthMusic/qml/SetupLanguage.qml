import QtQuick 2.8
import QtQuick.Layouts 1.3
import SvQml 1.0

SvTabViewItem {

  Column {
    y : 70
    width: parent.width
    spacing: 10

    //Interface language selection
    ListView {
      height: 110
      width: 120 * 3
      anchors.horizontalCenter: parent.horizontalCenter
      //width: parent.width
      orientation: ListView.Horizontal
      model: ListModel {
        ListElement {
          name: qsTr("Russian")
          image: "qrc:/img/RU.png"
        }
        ListElement {
          name: qsTr("English")
          image: "qrc:/img/EN.png"
        }
//        ListElement {
//          name: qsTr("German")
//          image: "qrc:/img/DE.png"
//        }
      }

      delegate: Item {
        width: 120
        height: 100
        ColumnLayout {
          anchors.fill: parent
          anchors.margins: 10
          spacing: 0
          Image {
            source: image
            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            fillMode: Image.PreserveAspectFit
          }
          Text {
            text: name
            font.pointSize: 12
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "white"
          }
        }
      }

    }
  }

}
