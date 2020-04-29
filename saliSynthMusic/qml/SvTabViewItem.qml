/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Страница для табулятора SvTabView
*/
import QtQuick 2.8
import QtQuick.Layouts 1.3

//базовый элемент для вклдаки с заголовком сверху, картинкой для панели вкладок и содержимым
Item {
  //Размещение названия по y
  property int    pageNameY: 5
  //Название вкладки на экране
  property string pageName: ""
  //Название вкладки на панели вклдаок
  property string tabName: pageName
  //Путь к картинке для панели вкладок
  property string tabImage

  //Вывести подпись названия вкладки на экране
  SvText {
    font.bold: true
    font.pointSize: 16
    text: pageName
    color: 'black'
    anchors.horizontalCenter: parent.horizontalCenter
    horizontalAlignment: Qt.AlignHCenter
    y: pageNameY
  }

}
