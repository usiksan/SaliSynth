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
  //Наименование функции, выполняющейся при входе на вкладку
  property string enterFunctionForward: ""
  property string enterFunctionBackward: enterFunctionForward


  //Механизм проверки возможности перехода к следующему шагу
  property bool isNextEnable : true
  signal testNextEnable()

  function testCover() {
    if( !isCoverClosed ) {
      isNextEnable = false;
      messageBox.info( qsTr("Закройте крышку и повторите"), null );
      return false;
    }
    return true;
  }

  //Вывести подпись названия вкладки на экране
  Text {
    font.bold: true
    font.pointSize: 16
    text: pageName
    color: 'white'
    anchors.horizontalCenter: parent.horizontalCenter
    horizontalAlignment: Qt.AlignHCenter
    y: pageNameY
  }

}
