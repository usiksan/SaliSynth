/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Окно ввода цифровых значений
*/
import QtQuick 2.8
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

SvModalItem {
  id: box

  //property var    editElement     //редактируемый элемент. При затемнении редактируемый элемент будет выделен
  property string prevValue; //Начальное значение редактируемого параметра

  //Функция, которая будет выполнена при ОК
  property var applyFunction

  //Функция вывода сообщения на экран
  function show( title, value, apply ) {
    //Установить сообщения
    titleId.text = title;
    field.text = value;
    field.selectAll();
    prevValue = value;
    //Сохранить функцию для последующего вызова
    applyFunction = apply;
    //Включить сообщение
    visible = true;
    field.focus = true;
  }

  //Картинка, представляющая фон
  Image {
    id: bkgImage
    anchors.centerIn: parent
    source : "qrc:/img/backNumPad.png"
    //Размер текста на кнопках 96 ширины
    property int text96size: 18

    function addDigit( dig ) {
      //Если есть выделение или есть ведущий нуль, то заменить весь текст
      if( field.selectedText.length || field.text == "0" )
        field.text = dig;
      //иначе добавить текст
      else
        field.text = field.text + dig
    }

    //Заголовок
    Text {
      id: titleId
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.top: parent.top
      anchors.topMargin: 3
      anchors.leftMargin: 20
      anchors.rightMargin: 20
      horizontalAlignment: Qt.AlignHCenter
      color: "#ffffff"
      font.bold: true
      font.pointSize: 14
      font.family: svStyle.mainFontFamily
    }



    //Редактируемое значение
    TextInput {
      id: field
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.top: parent.top
      anchors.topMargin: 39
      anchors.leftMargin: 50
      anchors.rightMargin: 50
      height: 50
      verticalAlignment: Qt.AlignHCenter
      horizontalAlignment: Qt.AlignRight

      clip: true
      font.bold: true
      font.family: svStyle.mainFontFamily
      font.pixelSize: 40

      //validator: RegExpValidator { regExp: "/\\-?\\d+(\\.\\d{0,})?/" }
      focus: true

      onAccepted: {
        //Закрыть окно
        box.visible = false;
        //Вернуть значение
        if( applyFunction != null )
          applyFunction( text )
        }
      Keys.onEscapePressed: {
        //Закрыть окно
        box.visible = false;
      }
    }

    //Поле клавиш
    GridLayout {
      anchors.fill: parent
      anchors.topMargin: 117
      anchors.leftMargin: 20
      anchors.rightMargin: 20
      anchors.bottomMargin: 20

      Item{ Layout.fillWidth: true; Layout.fillHeight: true; }
      Item{ Layout.row: 2; Layout.column: 2; Layout.fillWidth: true; Layout.fillHeight: true; }

      GridLayout {
        Layout.row: 1
        Layout.column: 1

        columnSpacing: 10
        rowSpacing: columnSpacing
        //Цифры
        SvNumPadKey64 {
          text: "0"
          onClick: bkgImage.addDigit( text )
          Layout.row: 3; Layout.column: 0
        }

        SvNumPadKey64 {
          text: "."
          onClick: if( field.text.indexOf(".") < 0 ) field.text = field.text + text
          Layout.row: 3; Layout.column: 1
        }

        SvNumPadKey64 {
          text : "-"
          onClick: {
            if( field.text.indexOf("-") < 0 )
              //Добавить минус
              field.text = "-" + field.text
            else
              //Убрать минус
              field.text = field.text.substring( 1 );
          }
          Layout.row: 3; Layout.column: 2
        }

        SvButtonImage {
          //Базовое название картинок
          imgSource: "qrc:/img/Kbd96"
          text : qsTr("Принять")
          textSize: bkgImage.text96size
          onClick: {
            //Закрыть окно
            box.visible = false;
            //Вернуть значение
            if( applyFunction != null )
              applyFunction( field.text )
          }
          Layout.row: 3; Layout.column: 3
        }


        SvNumPadKey64 {
          text: "1"
          onClick: bkgImage.addDigit( text )
          Layout.row: 2; Layout.column: 0
        }

        SvNumPadKey64 {
          text: "2"
          onClick: bkgImage.addDigit( text )
          Layout.row: 2; Layout.column: 1
        }

        SvNumPadKey64 {
          text: "3"
          onClick: bkgImage.addDigit( text )
          Layout.row: 2; Layout.column: 2
        }

        SvButtonImage {
          //Базовое название картинок
          imgSource: "qrc:/img/Kbd96"
          text : qsTr("Сброс")
          textSize: bkgImage.text96size
          onClick: field.text = prevValue;
          Layout.row: 2; Layout.column: 3
        }



        SvNumPadKey64 {
          text: "4"
          onClick: bkgImage.addDigit( text )
          Layout.row: 1; Layout.column: 0
        }

        SvNumPadKey64 {
          text: "5"
          onClick: bkgImage.addDigit( text )
          Layout.row: 1; Layout.column: 1
        }

        SvNumPadKey64 {
          text: "6"
          onClick: bkgImage.addDigit( text )
          Layout.row: 1; Layout.column: 2
        }

        SvButtonImage {
          //Базовое название картинок
          imgSource: "qrc:/img/Kbd96"
          text: qsTr("Закрыть")
          textSize: bkgImage.text96size
          onClick: box.visible = false;
          Layout.row: 1; Layout.column: 3
        }



        SvNumPadKey64 {
          text: "7"
          onClick: bkgImage.addDigit( text )
          Layout.row: 0; Layout.column: 0
          }

        SvNumPadKey64 {
          text: "8"
          onClick: bkgImage.addDigit( text )
          Layout.row: 0; Layout.column: 1
        }

        SvNumPadKey64 {
          text: "9"
          onClick: bkgImage.addDigit( text )
          Layout.row: 0; Layout.column: 2
        }



        SvButtonImage {
          //Базовое название картинок
          imgSource: "qrc:/img/Kbd96"
          text: qsTr("Стереть")
          textSize: bkgImage.text96size
          onClick: {
            if( field.text.length < 2 )
              field.text = "0";
            else
              field.text = field.text.substring( 0, field.text.length - 1 );
            }
          Layout.row: 0; Layout.column: 3
        }
      }
    }
  }
}
