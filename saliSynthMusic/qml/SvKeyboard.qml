/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Окно ввода строки текста
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
    visible = true
    //Активировать фокус строки ввода
    field.focus = true;
  }

  //Картинка, представляющая фон
  Rectangle {
    id: kbd
    width: 800
    height: 390
    anchors.centerIn: parent
    color: svStyle.keyboardBack
    //source : "qrc:/img/backKeyboard.png"

    //Флаги редактирования
    property bool  first: true
    property bool  eng: false
    property bool  shift: false
    property bool  caps: false

    function enter() {
      //Закрыть окно
      box.visible = false;
      //Вернуть значение
      if( applyFunction !== null )
        applyFunction( field.text )
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


    Rectangle {
      anchors.fill: field
      color: svStyle.keyboardEditBack
      anchors.margins: -5
    }

    //Редактируемое значение
    TextInput {
      id: field
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.top: parent.top
      anchors.topMargin: 8
      anchors.leftMargin: 50
      anchors.rightMargin: 50
      height: 50
      verticalAlignment: Qt.AlignHCenter
      horizontalAlignment: Qt.AlignLeft

      clip: true
      font.bold: true
      font.family: svStyle.mainFontFamily
      font.pixelSize: 40
      color: svStyle.keyboardEditText

      focus: true

      onAccepted: kbd.enter();

      Keys.onEscapePressed: {
        //Закрыть окно
        box.visible = false;
      }

      function insertText( key ) {
        //Если есть выделение или есть ведущий нуль, то заменить весь текст
        if( field.selectedText.length )
          field.text = key;
        //иначе добавить текст
        else
          field.text = field.text + key
          //insert( cursorPosition, key )
        kbd.shift = false;
      }
    }



    Column {
      y: 64
      spacing: 2
      //Кнопки расположены рядами
      //Esc, цифровые кнопки
      Row {
        spacing: 1
        SvKeyboardButton56x56 {
          keyHigh: qsTr("1")
          keyLow: keyHigh
          engHigh: qsTr("!")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("2")
          keyLow: keyHigh
          engHigh: qsTr("@")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("3")
          keyLow: keyHigh
          engHigh: qsTr("#")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("4")
          keyLow: keyHigh
          engHigh: qsTr("$")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("5")
          keyLow: keyHigh
          engHigh: qsTr("%")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("6")
          keyLow: keyHigh
          engHigh: qsTr("^")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("7")
          keyLow: keyHigh
          engHigh: qsTr("&")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("8")
          keyLow: keyHigh
          engHigh: qsTr("*")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("9")
          keyLow: keyHigh
          engHigh: qsTr("(")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("0")
          keyLow: keyHigh
          engHigh: qsTr(")")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("-")
          keyLow: keyHigh
          engHigh: qsTr("_")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("+")
          keyLow: keyHigh
          engHigh: qsTr("=")
          engLow: engHigh
          shift: false
          eng: kbd.shift
          caps: false
          onClick: field.insertText(key);
        }

      }

      //Tab, qwerty
      Row {
        spacing: 1

        SvKeyboardButton56x56 {
          keyHigh: qsTr("Й")
          keyLow: qsTr("й")
          engHigh: "Q"
          engLow: "q"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Ц")
          keyLow: qsTr("ц")
          engHigh: "W"
          engLow: "w"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("У")
          keyLow: qsTr("у")
          engHigh: "E"
          engLow: "e"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("К")
          keyLow: qsTr("к")
          engHigh: "R"
          engLow: "r"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Е")
          keyLow: qsTr("е")
          engHigh: "T"
          engLow: "t"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Н")
          keyLow: qsTr("н")
          engHigh: "Y"
          engLow: "y"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Г")
          keyLow: qsTr("г")
          engHigh: "U"
          engLow: "u"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Ш")
          keyLow: qsTr("ш")
          engHigh: "I"
          engLow: "i"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Щ")
          keyLow: qsTr("щ")
          engHigh: "O"
          engLow: "o"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("З")
          keyLow: qsTr("з")
          engHigh: "P"
          engLow: "p"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Х")
          keyLow: qsTr("х")
          engHigh: "{"
          engLow: "["
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Ъ")
          keyLow: qsTr("ъ")
          engHigh: "}"
          engLow: "]"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
      }

      //Caps, asdfgh
      Row {
        spacing: 1

        SvKeyboardButton56x56 {
          keyHigh: qsTr("Ф")
          keyLow: qsTr("ф")
          engHigh: "A"
          engLow: "a"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Ы")
          keyLow: qsTr("ы")
          engHigh: "S"
          engLow: "s"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("В")
          keyLow: qsTr("в")
          engHigh: "D"
          engLow: "d"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("А")
          keyLow: qsTr("а")
          engHigh: "F"
          engLow: "f"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("П")
          keyLow: qsTr("п")
          engHigh: "G"
          engLow: "g"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Р")
          keyLow: qsTr("р")
          engHigh: "H"
          engLow: "h"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("О")
          keyLow: qsTr("о")
          engHigh: "J"
          engLow: "j"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Л")
          keyLow: qsTr("л")
          engHigh: "K"
          engLow: "k"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Д")
          keyLow: qsTr("д")
          engHigh: "L"
          engLow: "l"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Ж")
          keyLow: qsTr("ж")
          engHigh: ":"
          engLow: ";"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Э")
          keyLow: qsTr("э")
          engHigh: "\""
          engLow: "\'"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56 {
          text: qsTr("Back")
          onClick: {
            //Удалить выделение
            field.select(0,0);
            field.text = field.text.substring( 0, field.text.length - 1 );
          }
        }
      }

      //Shift, zxcv
      Row {
        spacing: 1

        SvKeyboardButton56x56 {
          keyHigh: qsTr("/")
          keyLow: qsTr("|")
          engHigh: "/"
          engLow: "|"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Я")
          keyLow: qsTr("я")
          engHigh: "Z"
          engLow: "z"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Ч")
          keyLow: qsTr("ч")
          engHigh: "X"
          engLow: "x"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("С")
          keyLow: qsTr("с")
          engHigh: "C"
          engLow: "c"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("М")
          keyLow: qsTr("м")
          engHigh: "V"
          engLow: "v"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("И")
          keyLow: qsTr("и")
          engHigh: "B"
          engLow: "b"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Т")
          keyLow: qsTr("т")
          engHigh: "N"
          engLow: "n"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Ь")
          keyLow: qsTr("ь")
          engHigh: "M"
          engLow: "m"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Б")
          keyLow: qsTr("б")
          engHigh: "<"
          engLow: ","
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr("Ю")
          keyLow: qsTr("ю")
          engHigh: ">"
          engLow: "."
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56x56 {
          keyHigh: qsTr(".")
          keyLow: qsTr(",")
          engHigh: "?"
          engLow: "/"
          shift: kbd.shift
          eng: kbd.eng
          caps: kbd.caps
          onClick: field.insertText(key);
        }
        SvKeyboardButton56 {
          text: qsTr("Enter")
          onClick: kbd.enter()
        }
      }

      //Copy Paste Space
      Row {
        id: row5

        SvKeyboardButton56 {
          text: qsTr("Caps")
          onClick: kbd.caps = !kbd.caps;
        }
        SvKeyboardButton56 {
          text: qsTr("Shift")
          onClick: kbd.shift = !kbd.shift;
        }
        SvKeyboardButton56x56 {
          width: 100
          keyHigh: qsTr("Рус")
          keyLow: qsTr("Рус")
          engHigh: "Eng"
          engLow: "Eng"
          eng: kbd.eng
          onClick: kbd.eng = !kbd.eng;
        }
        SvKeyboardButton56 {
          width: 270
          onClick: field.insertText(" ");
        }
        SvKeyboardButton56 {
          text: qsTr("Clear")
          onClick: field.text = "";
        }
        SvKeyboardButton56 {
          text: qsTr("Shift")
          onClick: kbd.shift = !kbd.shift;
        }
        SvKeyboardButton56 {
          width: 100
          text: qsTr("ESC")
          onClick: box.visible = false;
        }
      }
    }
  }
}
