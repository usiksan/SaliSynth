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
  SvScrollView {
    anchors.top: idTitle.bottom
    anchors.topMargin: 5
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.right: parent.right
    anchors.rightMargin: 5
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 40

    clip: true

    ListView {
      anchors.fill: parent

      clip: true

      model: voiceList

      //Delegate to show single voice
      //with it we show also voice part title
      //When field voiceBankMsb contains "130" we show part title otherwise - show voice
      delegate: Item {
        id: rowDelegate
        property bool isCurrent : index === currentRow
        width: parent.width
        height: isCurrent ? 30 : 24
//        Rectangle {
//          anchors.fill: parent
//          anchors.topMargin: -3
//          visible: rowDelegate.isCurrent
//          color: Qt.lighter( "green" )
//        }

        //Part title
        Item {
          visible: voiceBankMsb === "130"
          anchors.fill: parent

          //Voice number
          SvFieldText {
            anchors.verticalCenter: parent.verticalCenter
            text: index + 1
            width: widthNpp
            horizontalAlignment: Text.AlignHCenter
            editable: false
            onLeftButton: setCurrentRow(index);
          }

          SvFieldText {
            anchors.centerIn: parent
            text: voiceName
            editable: true
            onLeftButton: setCurrentRow(index);
            onApply: voiceName = str;
          }

        }

        //Voice
        Row {
          visible: voiceBankMsb !== "130"
          spacing: 2
          anchors.verticalCenter: parent.verticalCenter

          //Voice number
          SvFieldText {
            text: index + 1
            width: widthNpp
            horizontalAlignment: Text.AlignHCenter
            editable: false
            onLeftButton: setCurrentRow(index);
          }

          //Voice icon
          Image {
            width: widthIcon
            height: 24
            source: voiceIconName
            fillMode: Image.PreserveAspectFit
            MouseArea {
              anchors.fill: parent
              acceptedButtons: Qt.LeftButton | Qt.RightButton
              onClicked: {
                setCurrentRow(index);
                instrumentIconSelector.instrumentIconSelect( voiceIconName, function (str) { voiceIconName = str; } )
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
            onApply: {
              if( !synth.voiceSettings( index, voiceBankMsb, str, voiceProgram ) )
                messageBox.error( qsTr("Duplicate voice, enter another bank lsb value"), null )
            }
          }

          //Midi programm
          SvFieldText {
            text: voiceProgram
            width: widthNpp
            editable: true
            onLeftButton: setCurrentRow(index);
            onApply: {
              if( !synth.voiceSettings( index, voiceBankMsb, voiceBankLsb, str ) )
                messageBox.error( qsTr("Duplicate voice, enter another programm value"), null )
            }
          }

          //Voice name
          SvFieldText {
            text: voiceName
            width: widthTitle
            editable: true
            onLeftButton: setCurrentRow(index);
            onApply: voiceName = str;
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
              soundFontSelector.selectFont(voiceSoundFontFile)
            }
          }
        }

        //Selecting rectangle
        Rectangle {
          anchors.fill: parent
          visible: rowDelegate.isCurrent
          color: "transparent"
          border.color: Qt.lighter( "green" )
          border.width: 2
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
    SvToolButton {
      icon.source: "qrc:/img/plus_red.png"
      ToolTip.text: qsTr("Add new record to the voice list")

      onClicked: synth.voiceAdd();
    }

    //Insert new record
    SvToolButton {
      icon.source: "qrc:/img/table_row_new.png"
      ToolTip.text: qsTr("Insert new record to current position of the voice list")

      onClicked: synth.voiceInsert(currentRow)
    }

    //Duplicate current record
    SvToolButton {
      icon.source: "qrc:/img/copy.png"
      ToolTip.text: qsTr("Duplicate current record")

      onClicked: synth.voiceDuplicate(currentRow);
    }

    //Continuously extract all presets
    SvToolButton {
      icon.source: "qrc:/img/extract.png"
      ToolTip.text: qsTr("Continuously extract all presets")

      onClicked: synth.voiceFontExtractAll(currentRow);
    }

    //Delete current record
    SvToolButton {
      icon.source: "qrc:/img/delete_red.png"
      ToolTip.text: qsTr("Remove current record from voice list")

      onClicked: voiceList.removeRecord( currentRow );
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
