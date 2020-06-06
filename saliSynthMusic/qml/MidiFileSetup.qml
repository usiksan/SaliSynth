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
import QtQuick.Dialogs 1.3

Item {
  id: midiConfig
  anchors.fill: parent
  property string ifNoTrackFound
  property int   voiceInfoHeight : 100             //Default track rectangle height
  property alias trackModel : trackRepeater.model  //Access to the track model
  property bool  enableUserPlay : false
  property int   userPlayTrack : -1

  //Display midi track info
  Column {
    Repeater {
      id: trackRepeater
      model: qmlMidiFileTrackModel

      Item {
        width: 300
        height: 32

        Row {
          anchors.fill: parent

          //Color rectangle
          Rectangle {
            id: colorRectangle
            width: 30
            height: 30
            property string midiTrackColor: trackColor
            color: midiTrackColor

            ToolTip {
              text: qsTr("Track color. Press to change")
              visible: hovered
              delay: 300
            }

            MouseArea {
              anchors.fill: parent
              onClicked: colorDialog.visible = true
            }

            ColorDialog {
              id: colorDialog
              title: "Please choose a color"
              currentColor: colorRectangle.midiTrackColor
              onAccepted: {
                visible = false
                trackColor = colorDialog.color
              }
              onRejected: visible = false;
            }
          }


          //Voice icon
          SvImagedButton {
            buttonSize: 30
            anchors.verticalCenter: parent.verticalCenter
            buttonSource: voiceList.asString( synth.voiceRowById(trackId), "voiceIconName" )

            ToolTip.text: qsTr("Track voice icon. Press to select voice")

            onClicked: voiceSelector.selectVoiceEx( trackId, function ( mode, row ) {
              trackId = voiceList.asInt( row, "voiceId" );
              //trackName = voiceList.asString( row, "voiceName")
            } );
          }


          //On-off track sound
          SvImagedButton {
            id: onOffButton
            buttonSize: 28
            borderWidth: 0
            anchors.verticalCenter: parent.verticalCenter
            buttonSource: trackOn != 0 ? "qrc:/img/sound.png" : "qrc:/img/soundOff.png"

            ToolTip.text: qsTr("On-off sound track. Press to switch")

            onClicked: trackOn = trackOn != 0 ? 0 : 1;
          }


          //Track visible button
          SvImagedButton {
            id: trackVisibleButton
            buttonSize: 28
            borderWidth: 0
            anchors.verticalCenter: parent.verticalCenter
            buttonSource: trackVisible != 0 ? "qrc:/img/lampGreen.png" : "qrc:/img/lampOff.png"

            ToolTip.text: qsTr("Visible track. Press to switch")

            onClicked: trackVisible = trackVisible != 0 ? 0 : 1;
          }

          //User play selector
          SvImagedButton {
            id: userPlaySelector
            buttonSize: 28
            borderColor: "green"
            borderWidth: userPlayTrack == index ? 2 : 0
            anchors.verticalCenter: parent.verticalCenter
            buttonSource: "qrc:/img/voice.png"

            ToolTip.text: qsTr("Play track by yourself. Press to switch")

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

          //Track volume regulator
          Slider {
            id: volumeSlider
            width: 100
            height: 30
            anchors.verticalCenter: parent.verticalCenter
            orientation: Qt.Horizontal
            ToolTip.text: qsTr("Track volume")
            ToolTip.visible: hovered
            ToolTip.delay: 300
            from: 0
            to: 127
            value: Number( trackVolume )
            onMoved: trackVolume = value.toFixed();
          }

          //Voice name
          SvText {
            anchors.verticalCenter: parent.verticalCenter
            text: " (" + ((trackId >> 14) & 0x7f) + "," + ((trackId >> 7) & 0x7f) + "," + ((trackId) & 0x7f) + ") " + synth.voiceName(trackId)
          }

          //Track title
          SvFieldText {
            anchors.verticalCenter: parent.verticalCenter
            text: " : " + trackName
            editable: true
            onApply: trackName = str;
            width: 150
          }


        }





      }
    }
  }


//  Drawer {
//      id: drawer
//      width: 0.66 * midiConfig.width
//      height: midiConfig.height

//      Label {
//          text: "Content goes here!"
//          anchors.centerIn: parent
//      }
//  }

  //If no midi file loaded or no midi tracks defined
  //we display this text
  SvText {
    anchors.centerIn: parent
    visible: trackModel.count === 0
    text: ifNoTrackFound
    font.pixelSize: 24
  }
}

