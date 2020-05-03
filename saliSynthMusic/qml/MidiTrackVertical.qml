import QtQuick 2.0
import QtQuick.Controls 2.5
import SvQml 1.0

Item {
  anchors.fill: parent

  property real factor : 0.5

  SvScrollView {
    anchors.top: parent.top
    anchors.topMargin: 5
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.right: parent.right
    anchors.rightMargin: 5
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 0

    Item {
      id: midiArea
      height: qmlMidiFile.fileLenght * factor
      width: parent.width

      //At first - white keys
      Repeater {
        model: qmlMidiFile.qmlMidiTrack(1)

        Rectangle {
          visible: mefType == 0x10 && !qmlKeyboard.keyIsBlack( mefNote );
          x: qmlKeyboard.whiteKeyWidth * qmlKeyboard.keyIndex( mefNote );
          width: qmlKeyboard.whiteKeyWidth
          y: (qmlMidiFile.fileLenght - mefTime - mefLenght) * factor
          height: mefLenght * factor

          color: "green"
          border.color: "black"
          border.width: 1
        }

      }

      //At second - black keys
      Repeater {
        model: qmlMidiFile.qmlMidiTrack(1)

        Rectangle {
          visible: mefType == 0x10 && qmlKeyboard.keyIsBlack( mefNote );
          x: (qmlKeyboard.whiteKeyWidth + width) / 2 + qmlKeyboard.whiteKeyWidth * qmlKeyboard.keyIndex( mefNote );
          width: qmlKeyboard.whiteKeyWidth / 2 + 2
          y: (qmlMidiFile.fileLenght - mefTime - mefLenght) * factor
          height: mefLenght * factor

          color: "green"
          border.color: "black"
          border.width: 1
        }

      }

    }
  }

}
