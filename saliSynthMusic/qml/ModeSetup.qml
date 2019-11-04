/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Setup mode
*/
import QtQuick 2.8
import QtQml.Models 2.2
import SvQml 1.0

Item {
  anchors.fill: parent

  SvTabView {
    textSize: 10
    tabs: ObjectModel {
      SetupLanguage {
        pageName: qsTr("Language")
        tabImage: "qrc:/img/iconLanguage.png"
      }
      SetupSoundFont {
        pageName: qsTr("Sound fonts")
        tabImage: "qrc:/img/iconSoundFont.png"
      }
//      SetupInputs {
//        pageName: qsTr("ÐÑÐ¾Ð´Ñ")
//        tabImage: "qrc:/img/iconInputs.png"
//      }
//      SetupOutputs {
//        pageName: qsTr("ÐÑÑÐ¾Ð´Ñ")
//        tabImage: "qrc:/img/iconOutputs.png"
//      }
//      SetupMotors {
//        tabName: qsTr("ÐÑÐ¸Ð²Ð¾Ð´Ñ")
//        tabImage: "qrc:/img/iconMotorList.png"
//      }
//      SetupCalibr {
//        pageName: qsTr("ÐÐ°Ð»Ð¸Ð±Ñ")
//        tabImage: "qrc:/img/iconCameraMap.png"
//      }
//      SetupFind {
//        pageName: qsTr("ÐÐ¾Ð¸ÑÐº")
//        tabImage: "qrc:/img/iconCameraMap.png"
//      }
//      SetupImpos {
//        pageName: qsTr("Ð¡Ð¾Ð²Ð¼ÐµÑÐµÐ½Ð¸Ðµ")
//        tabImage: "qrc:/img/iconImposition.png"
//      }
//      SetupClearing {
//        pageName: qsTr("Ð§Ð¸ÑÑÐºÐ° ")
//        tabImage: "qrc:/img/iconClearing.png"
//      }
//      SetupTimeouts {
//        pageName: qsTr("ÐÐ°Ð´ÐµÑÐ¶ÐºÐ¸")
//        tabImage: "qrc:/img/iconTimeOuts.png"
//      }
//      SetupMechanic {
//        pageName: qsTr("ÐÐµÑÐ°Ð½Ð¸ÐºÐ°")
//        tabImage: "qrc:/img/iconMechanic.png"
//      }
    }
  }

}