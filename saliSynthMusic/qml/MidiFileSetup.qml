import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
  id: midiConfig
  anchors.fill: parent
  property int voiceInfoHeight : 100
  property alias trackModel : trackRepeater.model

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
        border.color: "green"
        border.width: 2

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
            width: 64
            height: 64
            source: voiceList.asString( synth.voiceRowById(trackId), "voiceIconName" )
            fillMode: Image.PreserveAspectFit
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
            source: trackVisible != 0 ? "qrc:/img/sound.png" : "qrc:/img/soundOff.png"
            fillMode: Image.PreserveAspectFit
          }
          ToolTip.text: qsTr("Visible track. Press to switch")
          ToolTip.visible: hovered
          ToolTip.delay: 300

          onClicked: trackVisible = trackVisible != 0 ? 0 : 1;
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
    text: qsTr("No midi file loaded")
  }
}

