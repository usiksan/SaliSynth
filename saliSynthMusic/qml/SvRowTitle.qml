/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Заголовок для последюущих полей. Призван разделять блоки параметров.
*/
import QtQuick 2.8
import QtQuick.Layouts 1.3


Item {
  id: base
  height: parent.rowHeight
  width: parent.rowWidth

  //Имя поля
  property alias title: idTitle.text

  //Заголовок по центру
  SvText {
    id: idTitle

    //Цвет текста
    color: svStyle.fieldName

    //Размер текста
    font.pixelSize: svStyle.fieldNameSize

    anchors.horizontalCenter: parent.horizontalCenter
  }

}
