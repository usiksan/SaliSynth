/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Заголовок для поля
  Берет свойства из глобальных свойств
*/
import QtQuick 2.8

SvText {
  //Свойство разрешающее взаимодействие с полем
  property bool enable: true

  //Цвет текста
  color: enable ? svStyle.fieldName : svStyle.fieldNameDisable

  //Размер текста
  font.pixelSize: svStyle.fieldNameSize
}
