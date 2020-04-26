import QtQuick 2.0

Item {
  property int keys: 61

  //Ряд белых клавиш
  Repeater {
    model: qmlKeyboard

    Rectangle {
      visible: !keyIsBlack
      x: keyIndex * 22
      y: 0
      width: 22
      height: 50
      border.width: 1
      border.color: "black"
      color: "white"

      //Key pressed indication
      Rectangle {
        visible: keyColor & 3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        width: 8
        height: 8
        radius: 4
        border.width: 0
        color: "green"
      }

      MouseArea {
        anchors.fill: parent
        onPressed: keyColor = true
        onReleased: keyColor = false
      }
    }

  }

  //Ряд черных клавиш
  Repeater {
    model: qmlKeyboard

    Rectangle {
      visible: keyIsBlack
      x: 17 + keyIndex * 22
      y: 0
      width: 12
      height: 30
      color: "black"

      //Key pressed indication
      Rectangle {
        visible: keyColor & 3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        width: 8
        height: 8
        radius: 4
        border.width: 0
        color: "green"
      }

      MouseArea {
        anchors.fill: parent
        onPressed: keyColor = true
        onReleased: keyColor = false
      }
    }
  }

}
