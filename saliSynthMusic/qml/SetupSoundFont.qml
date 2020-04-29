/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   Setup
   Sound font table management
*/
import QtQuick 2.8
import QtQuick.Controls 2.5

SvTabViewItem {
  //Fields with
//  property int widthMarker: 50
  property int widthNpp: 40
  property int widthIcon: 64
  property int widthTitle: 150
  property int widthPreset: 150
  property int widthFontName: 250

  property int currentRow : 0

  function setCurrentRow( row ) {
    currentRow = row;
    synth.setProgramm( 0, row );
  }


  //Table title
  Row {
    id: idTitle
    anchors.top: parent.top
    anchors.topMargin: 40
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.right: parent.right
    anchors.rightMargin: 5
    height: 48

    spacing: 2
    //Select instrument
//    SvTableHeader {
//      text: qsTr("Select")
//      width: widthMarker
//    }
    //Programm number
    SvTableHeader {
      text: qsTr("№")
      width: widthNpp
    }
    //Instrument icon
    SvTableHeader {
      //text: qsTr("")
      width: widthIcon
    }
    //Bank MSB
    SvTableHeader {
      text: qsTr("Bank\nMSB")
      width: widthNpp
    }
    //Bank LSB
    SvTableHeader {
      text: qsTr("Bank\nLSB")
      width: widthNpp
    }
    //Midi programm
    SvTableHeader {
      text: qsTr("Prog")
      width: widthNpp
    }
    //Voice name
    SvTableHeader {
      text: qsTr("Title")
      width: widthTitle
    }
    //Preset name
    SvTableHeader {
      text: qsTr("Preset")
      width: widthPreset
    }
    //Font file name
    SvTableHeader {
      text: qsTr("Font file name")
      width: widthFontName
    }
  }

  //Preset from sound font
//  "voiceIconName",            //visual icon name for instrument
//  "voiceSoundFontFile",       //Sound font file name
//  "voiceSoundFontPreset",     //Preset in sound font file
//  "voiceSoundFontPresetName", //Preset name in sound font file

  //The voice that the preset is associated with
//  "voiceName",                //Arbitrary voice name
//  "voiceBankMsb",             //Voice bank MSB
//  "voiceBankLsb",             //Voice bank LSB
//  "voiceProgram",             //Voice bank midi program
  //Table contents
  ListView {
    anchors.top: idTitle.bottom
    anchors.topMargin: 5
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.right: parent.right
    anchors.rightMargin: 5
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 40

    clip: true

    model: voiceList

    delegate: Item {
      width: parent.width
      height: 24
      Rectangle {
        anchors.fill: parent
        anchors.topMargin: -3
        visible: index === currentRow
        color: Qt.lighter( "green" )
      }
      Row {
        spacing: 2
        anchors.verticalCenter: parent.verticalCenter

        //Voice number
        SvFieldText {
          text: index + 1
          width: widthNpp
          editable: false
          onLeftButton: setCurrentRow(index);
        }

        //Voice icon
        Image {
          width: widthIcon
          height: 24
          source: voiceIconName
          MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
              setCurrentRow(index);
              instrumentIconSelector.instrumentIconSelect( iconName, function (str) { iconName = str; } )
            }
          }
        }

        //Bank MSB
        SvFieldText {
          text: voiceBankMsb
          width: widthNpp
          editable: true
          onLeftButton: setCurrentRow(index);
          onApply: {
            if( !synth.voiceSettings( index, str, voiceBankLsb, voiceProgram ) )
              messageBox.error( qsTr("Duplicate voice, enter another bank msb value"), null )
          }
        }

        //Bank LSB
        SvFieldText {
          text: voiceBankLsb
          width: widthNpp
          editable: true
          onLeftButton: setCurrentRow(index);
          onApply: synth.voiceSettings( index, voiceBankMsb, str, voiceProgram )
        }

        //Midi programm
        SvFieldText {
          text: voiceProgram
          width: widthNpp
          editable: true
          onLeftButton: setCurrentRow(index);
          onApply: synth.voiceSettings( index, voiceBankMsb, voiceBankLsb, str )
        }

        //Voice name
        SvFieldText {
          text: voiceName
          width: widthTitle
          editable: true
          onLeftButton: setCurrentRow(index);
          onApply: instrumentTitle = str;
        }

        //Preset name
        SvFieldText {
          text: voiceSoundFontPresetName
          width: widthPreset
          editable: false
          onLeftButton: {
            setCurrentRow(index);
            presetListSelector.presetSelect( index, voiceSoundFontPreset );
          }
        }

        //Sound font file
        SvFieldText {
          text: voiceSoundFontFile
          width: widthFontName
          editable: false
          onLeftButton: {
            setCurrentRow(index);
            soundFontSelector.visible = true
          }
        }
      }
    }
  }

  //Local menu
  Row {
    anchors.top: parent.top
    anchors.right: parent.right
    height: 24
    spacing: 5

    //Append new record
    ToolButton {
      icon.source: "qrc:/img/plus_red.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Add new record to the voice list")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: synth.voiceAdd();
    }

    //Duplicate current record
    ToolButton {
      icon.source: "qrc:/img/copy.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Duplicate current record")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: synth.voiceDuplicate(currentRow);
    }

    //Delete current record
    ToolButton {
      icon.source: "qrc:/img/delete_red.png"
      icon.color: "transparent"
      ToolTip.text: qsTr("Remove current record from voice list")
      ToolTip.visible: hovered
      ToolTip.delay: 300

      onClicked: voiceMap.removeRecord( currentRow );
    }

  }


  SoundFontLoad {
    id: soundFontSelector
    onSelected: synth.applySoundFont( currentRow, str, 0 )
  }

  PresetListSelect {
    id: presetListSelector
    onSelected: synth.applyPreset( currentRow, presetIndex )
  }
}
