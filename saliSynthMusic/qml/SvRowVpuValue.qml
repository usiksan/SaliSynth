/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Поле с именем. Имя прибивается к левому краю, значение - к правому
  Цифровое значение из vpu, с возможностью редактирования
*/
import QtQuick 2.8
import SvQml 1.0


Item {
  id: base
  height: parent.rowHeight
  width: parent.rowWidth

  //Значение для редактирования
  property SvQmlValue value

  //Цвет изображения значения
  property alias valueColor: idValue.color

  //Позиция значения
  property int valuePos : parent.rowValuePos

  //Имя поля
  property alias title: idTitle.text

  //Разрешение поля
  property bool enable: true

  //Разрешение редактирования поля
  property alias editable : idValue.editable

  //Сигнал вызывается после завершения редактирования и установки нового значения
  signal afterApply()

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
  SvFieldNumber {
    id: idValue
    text: value.asString
    title: idTitle.text
    x: valuePos
    anchors.verticalCenter: parent.verticalCenter
    enable: parent.enable
    onApply: {
      //Присвоить новое значение
      value.asString = str;
      //Вызвать дополнительную функцию обработки
      base.afterApply();
    }
  }
}
