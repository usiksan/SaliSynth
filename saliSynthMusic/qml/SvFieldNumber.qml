/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Цифровое поле, возможно редактирование
  Для редактирования использует глобальный объект numPad
  Берет свойства из глобальных свойств
*/
import QtQuick 2.8

SvText {
  //Название поля
  property string title

  //Свойство разрешающее взаимодействие с полем
  property bool   enable: true

  //Свойство разрешающее редактирование значения
  property bool   editable: true

  //Сигнал при успешном завершении редактирования
  signal apply( string str )

  //Сигнал при нажатии левой кнопкой
  signal leftButton()

  //Сигнал при нажатии правой кнопкой
  signal rightButton()

  //Цвет текста
  color: enable ? svStyle.fieldDefault : svStyle.fieldDisable

  //Размер текста
  font.pixelSize: svStyle.fieldSize

  height: svStyle.fieldSize

  MouseArea {
    anchors.fill: parent
    enabled: enable
    acceptedButtons: Qt.LeftButton | Qt.RightButton
    onClicked: {
      if( mouse.button === Qt.LeftButton ) leftButton();
      if( mouse.button === Qt.RightButton ) rightButton();
      //Если разрешено редактирование, то вызвать клавиатуру для редактирования
      if( editable )
        numPad.show( title, text,  function ( result ) { apply( result ); }   );
    }
  }
}
