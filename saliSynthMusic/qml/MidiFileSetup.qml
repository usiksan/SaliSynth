/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Display midi per track information. Allow to tune individual tracks
   Each track displays as single rectangle. Info extrackted from trackModel with qmlMidiFileTrackModel as default model
*/
import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
  id: midiConfig
  anchors.fill: parent
  property string ifNoTrackFound
  property int   voiceInfoHeight : 100             //Default track rectangle height
  property alias trackModel : trackRepeater.model  //Access to the track model
  property bool  enableUserPlay : false
  property int   userPlayTrack : -1

  //Display midi track info
  Flow {
    anchors.fill: parent
    spacing: 5

    Repeater {
      id: trackRepeater
      model: qmlMidiFileTrackModel

      Rectangle {
        width: 190
        height: midiConfig.voiceInfoHeight
        property string midiTrackColor: trackColor
        border.color: midiTrackColor
        border.width: 3

        //Voice icon
        Button {
          width: 68
          height: 68
          anchors.top: parent.top
          anchors.topMargin: 3
          anchors.left: parent.left
          anchors.leftMargin: 3
          padding: 0
          contentItem: Image {
            id: buttonImage
            width: 64
            height: 64
            source: voiceList.asString( synth.voiceRowById(trackId), "voiceIconName" )
            fillMode: Image.PreserveAspectFit
          }
          background: Rectangle {
            width: 68
            height: 68
            color: "transparent"
            border.color: "black"
            border.width: buttonImage.source === "" ? 2 : 0
          }

          ToolTip.text: qsTr("Track voice icon. Press to select voice")
          ToolTip.visible: hovered
          ToolTip.delay: 300

          onClicked: voiceSelector.selectVoiceEx( trackId, function ( mode, row ) {
            trackId = voiceList.asInt( row, "voiceId" );
            trackName = voiceList.asString( row, "voiceName")
          } );
        }

        //On-off track sound
        Button {
          id: onOffButton
          width: 28
          height: 28
          anchors.top: parent.top
          anchors.topMargin: 3
          anchors.right: volumeSlider.left
          anchors.rightMargin: 3
          padding: 0
          contentItem: Image {
            width: 24
            height: 24
            source: trackOn != 0 ? "qrc:/img/sound.png" : "qrc:/img/soundOff.png"
            fillMode: Image.PreserveAspectFit
          }
          background: Rectangle {
            width: 28
            height: 28
            color: "transparent"
          }

          ToolTip.text: qsTr("On-off sound track. Press to switch")
          ToolTip.visible: hovered
          ToolTip.delay: 300

          onClicked: trackOn = trackOn != 0 ? 0 : 1;
        }

        //Track visible button
        Button {
          id: trackVisibleButton
          width: 28
          height: 28
          anchors.top: parent.top
          anchors.topMargin: 3
          anchors.right: onOffButton.left
          anchors.rightMargin: 3
          padding: 0
          contentItem: Image {
            width: 24
            height: 24
            source: trackVisible != 0 ? "qrc:/img/lampGreen.png" : "qrc:/img/lampOff.png"
            fillMode: Image.PreserveAspectFit
          }
          background: Rectangle {
            width: 28
            height: 28
            color: "transparent"
          }
          ToolTip.text: qsTr("Visible track. Press to switch")
          ToolTip.visible: hovered
          ToolTip.delay: 300

          onClicked: trackVisible = trackVisible != 0 ? 0 : 1;
        }

        //User play selector
        Button {
          id: userPlaySelector
          width: 28
          height: 28
          anchors.top: onOffButton.bottom
          anchors.topMargin: 3
          anchors.right: onOffButton.right
          anchors.rightMargin: 0
          padding: 0
          contentItem: Image {
            width: 24
            height: 24
            source: "qrc:/img/voice.png"
            fillMode: Image.PreserveAspectFit
          }
          background: Rectangle {
            width: 28
            height: 28
            color: "transparent"
            border.color: "green"
            border.width: userPlayTrack == index ? 2 : 0
          }
          ToolTip.text: qsTr("Play track by yourself. Press to switch")
          ToolTip.visible: hovered
          ToolTip.delay: 300

          onClicked: {
            if( userPlayTrack === index ) {
              userPlayTrack = -1;
              trackOn = 1;
            }
            else {
              userPlayTrack = index;
              trackOn = 0;
              synth.channelSetVoiceId( 0, trackId );
            }
          }
        }




        //Track remark
        SvFieldText {
          text: trackRemark
          editable: true
          anchors.left: parent.left
          anchors.right: parent.right
          anchors.bottom: trackTitle.top
          anchors.margins: 3
          onApply: trackRemark = str;
        }


        //Track title
        SvFieldText {
          id: trackTitle
          text: trackName
          editable: true
          anchors.left: parent.left
          anchors.right: parent.right
          anchors.bottom: parent.bottom
          anchors.margins: 3
          //onLeftButton: setCurrentRow(index);
          onApply: trackName = str;
        }


        //Track volume regulator
        Slider {
          id: volumeSlider
          anchors.top: parent.top
          //anchors.topMargin: 33
          anchors.right: parent.right
          anchors.rightMargin: 3
          anchors.bottom: parent.bottom
          orientation: Qt.Vertical
          ToolTip.text: qsTr("Track volume")
          ToolTip.visible: hovered
          ToolTip.delay: 300
          from: 0
          to: 127
          value: Number( trackVolume )
          onMoved: trackVolume = value.toFixed();
        }

      }
    }
  }

  //If no midi file loaded or no midi tracks defined
  //we display this text
  SvText {
    anchors.centerIn: parent
    visible: trackModel.count === 0
    text: ifNoTrackFound
    font.pixelSize: 24
  }
}

