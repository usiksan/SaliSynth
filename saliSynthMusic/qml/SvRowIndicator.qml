/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Поле с именем. Имя прибивается к левому краю, значение - к правому
  Индикатор, с возможностью редактирования
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
  property alias isActive: idValue.isActive

  //Разрешение поля
  property bool enable: true

  //Нажатие на индикатор
  signal click()

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
  SvFieldIndicator {
    id: idValue
    x: valuePos
    anchors.verticalCenter: parent.verticalCenter
    enable: parent.enable
    onClick: base.click()
  }
}
