/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Окно запросов с ожиданием ответа пользователя

  Идея в передаче в качестве параметра функции, которая должна выполниться при
  нажатии пользователем на ту или иную кнопку
*/
import QtQuick 2.8
import QtQuick.Layouts 1.3

SvModalItem {
  id: box

  //Функция, которая будет выполнена при Yes
  property var yesFunction
  //Функция, которая будет выполнена при No
  property var noFunction

  //Функция вывода сообщения на экран
  function yesNoCancel( title, msg, yes, no ) {
    //Установить сообщения
    titleId.text = title;
    messageId.text = msg;
    //Сохранить функции для последующего вызова
    yesFunction = yes;
    noFunction = no;
    //Включить кнопку Отмена
    cancelButton.visible = true;
    //Включить сообщение
    visible = true
  }

  //Функция вывода сообщения на экран
  function yesNo( title, msg, yes, no ) {
    //Установить сообщения
    titleId.text = title;
    messageId.text = msg;
    //Сохранить функции для последующего вызова
    yesFunction = yes;
    noFunction = no;
    //Включить кнопку Отмена
    cancelButton.visible = false;
    //Включить сообщение
    visible = true
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
      anchors.bottom: buttons.top
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.margins: 10
      wrapMode: Text.WordWrap
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      color: svStyle.messageBoxMsg
      font.pixelSize: svStyle.messageBoxMsgSize
    }

    //Ряд кнопок внизу
    RowLayout {
      id: buttons
      height: 40
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.bottom: parent.bottom
      anchors.bottomMargin: 10

      //Кнопка Да
      SvButtonWithText {
        Layout.minimumWidth: 120
        text: qsTr("OK")
        onClick: {
          //Выключить сообщение
          box.visible = false;
          //Вызвать функцию, если есть
          //она также может включить сообщение
          if( yesFunction != null ) yesFunction();
        }
      }

      //Кнопка Нет
      SvButtonWithText {
        Layout.minimumWidth: 120
        text: qsTr("Нет")
        onClick: {
          //Выключить сообщение
          box.visible = false;
          //Вызвать функцию, если есть
          //она также может включить сообщение
          if( noFunction != null ) noFunction();
        }
      }

      //Кнопка Отмена
      SvButtonWithText {
        id: cancelButton
        Layout.minimumWidth: 120
        text: qsTr("Отмена")
        onClick: {
          //Выключить сообщение
          box.visible = false;
        }
      }

    }
  }
}

