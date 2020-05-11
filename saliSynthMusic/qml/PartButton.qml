import QtQuick 2.5
import QtQuick.Controls 2.5

Button {
  property int  mask           //Part mask
  property bool isMain : false //Indicator for main part
  width: qmlKeyboard.keyboardWidth / 12 - 1
  enabled: qmlStyleFile.parts & mask
  onClicked: qmlStyleFile.playPart(mask)
  Rectangle {
    anchors.fill: parent
    color: "transparent"
    border.color: "blue"
    border.width: 2
    visible: isMain && qmlStyleFile.mainPart & mask
  }

  Rectangle {
    anchors.fill: parent
    color: "transparent"
    border.color: "green"
    border.width: 2
    visible: qmlStyleFile.curPart & mask
  }
}
