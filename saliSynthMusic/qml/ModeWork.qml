import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3


Item {
  anchors.fill: parent

  //Work mode separated on two submodes: keyboard and acompaniment
  Item {
    id: workArea
    //Fill all area except keyboard
    anchors.left: pianoKeyboard.left
    anchors.right: pianoKeyboard.right
    anchors.top: parent.top
    anchors.bottom: pianoKeyboard.top

    WorkKey {
      visible: (qmlKeyboard.leftMode & 0x2) == 0
    }
    WorkAcompaniment {
      visible: (qmlKeyboard.leftMode & 0x2) != 0
    }
  }



  PianoKeyboard {
    id: pianoKeyboard
    anchors.left: parent.left
    anchors.leftMargin: 3
    anchors.right: parent.right
    anchors.bottom: parent.bottom
  }

  VoiceSelect {
    id: voiceSelector
    anchors.fill: parent
    anchors.bottomMargin: pianoKeyboard.height
  }
}
