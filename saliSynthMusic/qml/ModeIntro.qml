/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Intro page with work mode selection
*/
import QtQuick 2.0

Item {
  id: introMode
  anchors.fill: parent

  property int buttonSize: 70

  //Setup mode
  SvButtonImageWithText {
    img: "qrc:/img/service"
    legend: qsTr("Setup")
    buttonWidth: introMode.buttonSize
    buttonHeight: introMode.buttonSize
    enable: true
    onClicked: currentMode = 1;
   }


}
