/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Цифровое поле отображения значения из vpu
  Для редактирования использует глобальный объект numPad
  Берет свойства из глобальных свойств
*/
import QtQuick 2.8
import SvQml 1.0

SvFieldNumber {
  property SvQmlValue value //Значение для редактирования

  signal afterApply

  text: value.asString

  onApply: {
    //Установить введенное значение
    value.asString = str;
    //Выполнить действие после установки значения
    afterApply();
  }
}
