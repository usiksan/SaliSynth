/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Поле с именем. Имя прибивается к левому краю, значение - к правому
  Всплывающий список с возможностью выбора
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

  //То, что отображается в поле
  property alias valueDisplay: idValue.displayText

  //Значение поля в текстовом виде
  property alias valueText: idValue.currentText

  //Текущий индекс выбора
  property int valueIndex: 0
  onValueIndexChanged: idValue.currentIndex = valueIndex

  //Разрешение поля
  property bool enable: true

  //Модель для списка альтернатив
  property alias model : idValue.model

  //При нажатии на кнопку
  signal activated( int index )

  //С левой стороны имя поля
  SvFieldName {
    id: idTitle
    width: valuePos - 10
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.verticalCenter: parent.verticalCenter
    enable: parent.enable
  }

  //С правой стороны - выпадающий список выбора
  SvComboBox {
    id: idValue
    x: valuePos
    width: parent.width - valuePos
    height: parent.height
    anchors.verticalCenter: parent.verticalCenter
    onActivated: base.activated( index )
  }

}


