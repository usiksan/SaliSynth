/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Volume regulator control
*/
import QtQuick 2.14
import QtQuick.Controls 2.5

Rectangle {
  id: volumeSlider
  height: 100
  width: 28
  color: "light gray"
  border.color: "black"
  border.width: 1
  visible: false;
  property int volumeValue

  signal domoved( int volume )

  onVisibleChanged: {
    if( visible )
      offTimer.start();
  }

  Slider {
    anchors.fill: parent
    anchors.margins: 2
    orientation: Qt.Vertical
    from: 0
    to: 127

    value: volumeValue
    onMoved: domoved( value.toFixed() );
  }


  Timer {
    id: offTimer
    interval: 10000
    repeat: false
    onTriggered: volumeSlider.visible = false;
  }
}
