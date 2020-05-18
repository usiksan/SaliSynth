import QtQuick 2.5
import QtQuick.Controls 2.5

SpinBox {
  id: control
  ToolTip.visible: hovered
  ToolTip.delay: 300

  contentItem: TextInput {
    z: 2
    text: control.textFromValue(control.value, control.locale)
    font: control.font
    color: "white"
    selectionColor: "#21be2b"
    selectedTextColor: "#ffffff"
    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment: Qt.AlignVCenter

    readOnly: !control.editable
    validator: control.validator
    inputMethodHints: Qt.ImhFormattedNumbersOnly
  }

  up.indicator: Rectangle {
    x: parent.width - width
    height: parent.height
    implicitWidth: 26
    implicitHeight: 26
    color: control.up.pressed ? "#1D7FCF" : "#3BADE8"
    border.color: "#002989"

    Text {
      text: "+"
      font.pixelSize: control.font.pixelSize * 2
      color: "white"
      anchors.fill: parent
      fontSizeMode: Text.Fit
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
    }
  }

  down.indicator: Rectangle {
    x: 0
    height: parent.height
    implicitWidth: 26
    implicitHeight: 26
    color: control.down.pressed ? "#1D7FCF" : "#3BADE8"
    border.color: "#002989"

    Text {
      text: "-"
      font.pixelSize: control.font.pixelSize * 2
      color: "white"
      anchors.fill: parent
      fontSizeMode: Text.Fit
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
    }
  }

  background: Rectangle {
    implicitWidth: 100
    color: "#70C9E9"
    border.color: "#002989"
  }

}
