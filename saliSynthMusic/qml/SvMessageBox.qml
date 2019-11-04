/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Окно сообщений
*/
import QtQuick 2.8

SvModalItem {
  id: box

  //Функция, которая будет выполнена при ОК
  property var okFunction : null

  //Функция вывода сообщения на экран
  function show( title, msg, ok ) {
    //Установить сообщения
    titleId.text = title;
    messageId.text = msg;
    //Сохранить функцию для последующего вызова
    okFunction = ok;
    //Включить сообщение
    visible = true
  }

  //Функция вывода информационного сообщения
  function info( msg, ok ) {
    //Вывести сообщение на экран с предопределенным заголовком
    show( qsTr("Внимание"), msg, ok );
  }

  //Функция вывода ошибки
  function error( msg, ok ) {
    //Вывести сообщение на экран с предопределенным заголовком
    show( qsTr("Ошибка"), msg, ok );
  }


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
      anchors.bottom: okButton.top
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.margins: 10
      wrapMode: Text.WordWrap
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      color: svStyle.messageBoxMsg
      font.pixelSize: svStyle.messageBoxMsgSize
    }

    //Кнопка ОК
    SvButtonWithText {
      id: okButton
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.bottom: parent.bottom
      anchors.bottomMargin: 10
      text: qsTr("OK")
      width: 100
      height: 40
      onClick: {
        //Выключить сообщение
        box.visible = false;
        //Вызвать функцию, если есть
        //она также может включить сообщение
        if( okFunction != null ) okFunction();
      }
    }
  }
}

