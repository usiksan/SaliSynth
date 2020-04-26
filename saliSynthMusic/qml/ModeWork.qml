import QtQuick 2.0

Item {
  anchors.fill: parent

  PianoKeyboard {
    height: 60
    anchors.left: parent.left
    anchors.leftMargin: 3
    anchors.right: parent.right
    anchors.bottom: parent.bottom
  }
}
