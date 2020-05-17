/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Main work mode
*/
import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3


Item {
  anchors.fill: parent

  //Work mode separated on two submodes: keyboard and acompaniment
  Item {
    id: workArea
    visible: !voiceSelector.visible
    //Fill all area except keyboard
    anchors.left: pianoKeyboard.left
    anchors.right: pianoKeyboard.right
    anchors.top: parent.top
    anchors.bottom: pianoKeyboard.top

    WorkMidi {
      visible: (qmlKeyboard.leftMode & 0x2) == 0
    }
    WorkAccompaniment {
      visible: (qmlKeyboard.leftMode & 0x2) != 0
    }
  }



  PianoKeyboard {
    id: pianoKeyboard
    visible: !voiceSelector.visible
    anchors.left: parent.left
    anchors.leftMargin: 3
    anchors.right: parent.right
    anchors.bottom: parent.bottom
  }

  VoiceSelect {
    id: voiceSelector
    anchors.fill: parent
  }
}
