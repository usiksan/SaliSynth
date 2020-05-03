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
  ListElement { iconSrc: ""; iconTitle: qsTr("No icon") }
  ListElement { iconSrc: "qrc:/img/instrumentAccordion.png"; iconTitle: qsTr("Accordion") }
  ListElement { iconSrc: "qrc:/img/instrumentAcousticBass.png"; iconTitle: qsTr("Acoustic Bass") }
  ListElement { iconSrc: "qrc:/img/instrumentBassGuitar.png"; iconTitle: qsTr("Bass Guitar") }
  ListElement { iconSrc: "qrc:/img/instrumentCelesta.png"; iconTitle: qsTr("Celesta") }
  ListElement { iconSrc: "qrc:/img/instrumentChurchOrgan.png"; iconTitle: qsTr("Church Organ") }

  ListElement { iconSrc: "qrc:/img/instrumentClarinet.png"; iconTitle: qsTr("Clarinet") }
  ListElement { iconSrc: "qrc:/img/instrumentClavinet.png"; iconTitle: qsTr("Clavinet") }
  ListElement { iconSrc: "qrc:/img/instrumentDrawbarOrgan.png"; iconTitle: qsTr("Drawbar Organ") }
  ListElement { iconSrc: "qrc:/img/instrumentDrum.png"; iconTitle: qsTr("Drum") }
  ListElement { iconSrc: "qrc:/img/instrumentDulcimer.png"; iconTitle: qsTr("Dulcimer") }
  ListElement { iconSrc: "qrc:/img/instrumentGlockenspiel.png"; iconTitle: qsTr("Glockenspiel") }
  ListElement { iconSrc: "qrc:/img/instrumentGuitarAcoustic.png"; iconTitle: qsTr("Acoustic guitar") }
  ListElement { iconSrc: "qrc:/img/instrumentGuitarElectro.png"; iconTitle: qsTr("Electro guitar") }
  ListElement { iconSrc: "qrc:/img/instrumentHarmonica.png"; iconTitle: qsTr("Harmonica") }
  ListElement { iconSrc: "qrc:/img/instrumentHorn.png"; iconTitle: qsTr("Horn") }
  ListElement { iconSrc: "qrc:/img/instrumentMarimba.png"; iconTitle: qsTr("Marimba") }
  ListElement { iconSrc: "qrc:/img/instrumentMusicBox.png"; iconTitle: qsTr("Music Box") }
  ListElement { iconSrc: "qrc:/img/instrumentPiano.png"; iconTitle: qsTr("Piano") }
  ListElement { iconSrc: "qrc:/img/instrumentPipeOrgan.png"; iconTitle: qsTr("Pipe Organ") }
  ListElement { iconSrc: "qrc:/img/instrumentReedOrgan.png"; iconTitle: qsTr("Reed Organ") }

  ListElement { iconSrc: "qrc:/img/instrumentRockOrgan.png"; iconTitle: qsTr("Rock Organ") }
  ListElement { iconSrc: "qrc:/img/instrumentTubularBells.png"; iconTitle: qsTr("Tubular Bells") }
  ListElement { iconSrc: "qrc:/img/instrumentVibraphone.png"; iconTitle: qsTr("Vibraphone") }
  ListElement { iconSrc: "qrc:/img/instrumentXylophone.png"; iconTitle: qsTr("Xylophone") }
}
