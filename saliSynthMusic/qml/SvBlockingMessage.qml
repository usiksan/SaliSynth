/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Окно сообщений без возможности закрытия
*/
import QtQuick 2.8

SvModalItem {
  id: box

  property alias title : titleId.text
  property alias message : messageId.text

  //Здесь собственно содержимое окна
  Rectangle {
    anchors.centerIn: parent
    width: 400
    height: 250
    color: svStyle.messageBoxBack
    border.color: svStyle.messageBoxBorder
    border.width: 2


    //Заголовок выровнен посередине
    SvText {
      id: titleId
      anchors.top: parent.top
      anchors.margins: 10
      anchors.horizontalCenter: parent.horizontalCenter
      width: parent.width
      horizontalAlignment: Text.AlignHCenter
      color: svStyle.messageBoxTitle
      font.pixelSize: svStyle.messageBoxTitleSize
    }

    //Текст сообщения
    SvText {
      id: messageId
      anchors.top: titleId.bottom
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.margins: 10
      wrapMode: Text.WordWrap
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      color: svStyle.messageBoxMsg
      font.pixelSize: svStyle.messageBoxMsgSize
    }
  }
}

