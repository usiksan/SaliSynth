/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Display track notes as vertical rectangles
*/
import QtQuick 2.0
import QtQuick.Controls 2.5
import SvQml 1.0

Item {
  id: midiVerticalViewer
  anchors.fill: parent

  property real factor : 0.5

  //With this we support current play point display
  property int tickCount : qmlMidiFile.tickCount
  onTickCountChanged: {
    midiScroll.flickableItem.contentY = (qmlMidiFile.fileLenght - tickCount) * factor
  }

  SvScrollView {
    id: midiScroll
    anchors.top: parent.top
    anchors.topMargin: 5
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 0

    Item {
      id: midiArea
      height: qmlMidiFile.fileLenght * factor + midiScroll.height
      width: parent.width

      //For each existing tracks draw notes
      Repeater {
        model: qmlMidiFileTrackModel

        //This item is one track
        Item {
          id: trackArea
          anchors.fill: midiArea
          visible: trackVisible != 0

          property int midiVisualIndex: index              //Index of track in visual list
          property int midiTrackIndex: Number(trackIndex)  //Index of QmlMidiTrack in the qmlMidiTrackList
          property string midiTrackColor: trackColor

          //At first - white keys
          Repeater {
            model: qmlMidiTrackList[trackArea.midiTrackIndex]

            Rectangle {
              visible: mefType == 0x10 && !qmlKeyboard.keyIsBlack( mefNote );
              x: qmlKeyboard.whiteKeyWidth * qmlKeyboard.keyIndex( mefNote );
              width: qmlKeyboard.whiteKeyWidth
              y: (qmlMidiFile.fileLenght - mefTime - mefLenght) * factor + midiScroll.height
              height: mefLenght * factor

              color: trackArea.midiTrackColor
              border.color: "black"
              border.width: 1
            }

          }

          //At second - black keys
          Repeater {
            model: qmlMidiTrackList[trackArea.midiTrackIndex]

            Rectangle {
              visible: mefType == 0x10 && qmlKeyboard.keyIsBlack( mefNote );
              x: (qmlKeyboard.whiteKeyWidth + width) / 2 + qmlKeyboard.whiteKeyWidth * qmlKeyboard.keyIndex( mefNote );
              width: qmlKeyboard.whiteKeyWidth / 2 + 2
              y: (qmlMidiFile.fileLenght - mefTime - mefLenght) * factor + midiScroll.height
              height: mefLenght * factor

              color: trackArea.midiTrackColor
              border.color: "black"
              border.width: 1
            }

          }

        }
      }
    }
  }

//  SvText {
//    text: "position x:" + midiScroll.flickableItem.contentX + "  y:" + midiScroll.flickableItem.contentY
//  }

}
