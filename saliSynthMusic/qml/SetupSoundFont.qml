import QtQuick 2.0

SvTabViewItem {
  //Fields with
//  property int widthMarker: 50
  property int widthNpp: 50
  property int widthIcon: 70
  property int widthTitle: 150
  property int widthPreset: 50
  property int widthFontName: 250

   //Table title
  Row {
    id: idTitle
    anchors.top: parent.bottom
    anchors.topMargin: 35
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.right: parent.right
    anchors.rightMargin: 5
    height: 64

    spacing: 2
    //Select instrument
//    SvTableHeader {
//      text: qsTr("Select")
//      width: widthMarker
//    }
    //Programm number
    SvTableHeader {
      text: qsTr("Nprog")
      width: widthNpp
    }
    //Instrument icon
    SvTableHeader {
      //text: qsTr("")
      width: widthIcon
    }
    //Programm title
    SvTableHeader {
      text: qsTr("Title")
      width: widthTitle
    }
    //Preset
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

//  "iconName",        //visual icon name for instrument
//  "instrumentTitle", //instrument visual title
//  "soundFontFile",   //Sound font file name
//  "preset",          //Preset in sound font file
//  "programm"         //Programm which associates with this preset and sound font file
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

    model: soundFontMap

    delegate: Item {
      width: parent.width
      height: 64
      Rectangle {
        anchors.fill: parent
        visible: index === currentRow
        color: Qt.lighter( svStyle.backColor )
      }
      Row {
        spacing: 2
        anchors.verticalCenter: parent.verticalCenter
        //Programm number
        SvFieldText {
          text: programm
          width: widthNpp
          editable: false
          onLeftButton: setCurrentRow(index);
        }
        Image {
          width: 64
          height: 64
          source: iconName
        }
        //Instrument title
        SvFieldText {
          text: instrumentTitle
          width: widthTitle
          editable: true
          onLeftButton: setCurrentRow(index);
        }
        //Preset number
        SvFieldText {
          text: preset
          width: widthPart
          editable: true
          onLeftButton: setCurrentRow(index);
        }
        //Sound font file
        SvFieldText {
          text: soundFontFile
          width: widthFontName
          editable: true
          onLeftButton: setCurrentRow(index);
        }
      }
    }
  }
}
