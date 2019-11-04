/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Заголовок колонок в таблицах

  Изменяем размер текста и выравнивание для заголовков - посередине
*/
import QtQuick 2.0

SvFieldName {
  //Размер текста
  font.pixelSize: svStyle.fieldSize
  horizontalAlignment: Text.AlignHCenter
}
