/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Поле с именем. Имя прибивается к левому краю, значение - к правому
  Текстовое, с возможностью редактирования
*/
import QtQuick 2.8
import QtQuick.Layouts 1.3


Item {
  id: base
  height: parent.rowHeight
  width: parent.rowWidth

  //Позиция значения
  property int valuePos : parent.rowValuePos

  //Имя поля
  property alias title: idTitle.text

  //Значение поля
  property alias value: idValue.text

  //Цвет значения поля
  property alias valueColor: idValue.color

  //Разрешение поля
  property bool enable: true

  //Разрешение редактирования поля
  property alias editable : idValue.editable

  //Завершение редактирования
  signal apply( string str )

  //Нажатие левой кнопки
  signal leftButtonPressed()

  //Нажатие правой кнопки мыши
  signal rightButtonPressed()

  //С левой стороны имя поля
  SvFieldName {
    id: idTitle
    width: valuePos - 10
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.verticalCenter: parent.verticalCenter
    enable: parent.enable
  }

  //С правой стороны - текстовое поле значения с возможностью редактирования
  SvFieldText {
    id: idValue
    x: valuePos
    width: parent.width - x;
    anchors.verticalCenter: parent.verticalCenter
    enable: parent.enable
    onApply: base.apply( str )
    onLeftButton: base.leftButtonPressed();
    onRightButton: base.rightButtonPressed();
  }
}
