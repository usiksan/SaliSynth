/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Поле-индикатор состояния, возможно редактирование
  Берет свойства из глобальных свойств
*/
import QtQuick 2.8

//Индикатор представлен в виде прямоугольника
//При активном состоянии внутренность закрашена цветом активности
Rectangle {
  //Свойство активности индикатора
  property bool isActive: false

  //Свойство разрешающее взаимодействие с полем
  property bool enable: true

  //Размер индикатора
  property int  size: svStyle.fieldSize

  //Вызывается при нажатии на индикатор для изменения его состояния
  signal click()

  width: size
  height: size

  //Граница, ее цвет зависит от "редактируемости" индикатора
  border.width: 2
  border.color: enable ? svStyle.fieldDefault : svStyle.fieldDisable

  color: "transparent"

  //Точка, отображающая активность выхода
  Rectangle{
    anchors.fill: parent
    anchors.margins: 5
    color: isActive ? svStyle.fieldIndicatorActive : svStyle.fieldIndicatorPassive
    radius: width/2
  }

  //Область нажатия на индикатор
  MouseArea{
    anchors.fill: parent
    onClicked: if( enable ) click()
  }

}
