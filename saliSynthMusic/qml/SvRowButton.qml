/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Поле с именем. Имя прибивается к левому краю, значение - к правому
  Кнопка, с возможностью нажатия
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

  //Разрешение поля
  property bool enable: true

  //При нажатии на кнопку
  signal click()

  //Цвет бордюра кнопки
  property alias borderDefault : idValue.borderDefault

  //С левой стороны имя поля
  SvFieldName {
    id: idTitle
    width: valuePos - 10
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.verticalCenter: parent.verticalCenter
    enable: parent.enable
  }

  //С правой стороны - кнопка
  SvButtonWithText {
    id: idValue
    x: valuePos
    width: parent.width - valuePos
    height: parent.height
    anchors.verticalCenter: parent.verticalCenter
    enable: parent.enable
    text: qsTr("Старт")
    onClick: base.click();
  }
}
