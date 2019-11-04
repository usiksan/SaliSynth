/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Instrument icon list model
*/
import QtQuick 2.8

ListModel {
  ListElement {
    part: QT_TR_NOOP("Piano")
    programmIndex: 0
    title: QT_TR_NOOP("Grand piano")
  }
  ListElement {
    part: QT_TR_NOOP("Piano")
    programmIndex: 1
    title: QT_TR_NOOP("Bright piano")
  }
  ListElement {
    part: QT_TR_NOOP("Piano")
    programmIndex: 2
    title: QT_TR_NOOP("E Grand piano")
  }
  ListElement {
    part: QT_TR_NOOP("Piano")
    programmIndex: 3
    title: QT_TR_NOOP("Honky-tonk")
  }
  ListElement {
    part: QT_TR_NOOP("Piano")
    programmIndex: 2
    title: QT_TR_NOOP("E Grand piano")
  }


  ListElement {
    part: QT_TR_NOOP("Mallet")
  }

}
