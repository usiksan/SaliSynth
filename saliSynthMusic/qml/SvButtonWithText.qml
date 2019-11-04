/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Простая кнопка без картинки, но с подписью
*/
import QtQuick 2.0

Rectangle {
  //Цвета прямоугольных кнопок
  property color  backDefault: svStyle.buttonBackDefault
  property color  backDisable: svStyle.buttonBackDisable
  property color  backPressed: svStyle.buttonBackPressed
  property color  borderDefault: svStyle.buttonBorderDefault
  property color  borderDisable: svStyle.buttonBorderDisable
  property color  borderPressed: svStyle.buttonBorderPressed
  property color  textDefault: svStyle.buttonTextDefault
  property color  textDisable: svStyle.buttonTextDisable
  property color  textPressed: svStyle.buttonTextPressed
  property int    textSize: svStyle.buttonTextSize
  property alias  text : label.text

  //Разрешение кнопки
  property bool   enable: true
  //Кнопка нажата
  property bool   pressed: false

  //Сигнал при нажатии на кнопку
  signal click()

  width: 100
  height: 30

  color: enable ? (pressed ? backPressed : backDefault) : backDisable
  border.color: enable ? (pressed ? borderPressed : borderDefault) : borderDisable

  //Подпись кнопки
  SvText {
    id: label
    font.family: svStyle.buttonFontFamily
    font.pixelSize: textSize
    anchors.fill: parent
    anchors.margins: 4
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    color: enable ? (pressed ? textPressed : textDefault) : textDisable
  }

  //Анимация нажатия на кнопку (инициируется мышью)
  SequentialAnimation on pressed {
    id: pressAnim
    loops: 1
    //Последовательно выполняем цикл
    //нажатия
    PropertyAction { value: true }
    PauseAnimation { duration: 300 }
    //и отпускания
    PropertyAction { value: false }
  }

  //Область нажатия
  MouseArea {
    anchors.fill: parent
    enabled: enable
    onPressed: {
      pressAnim.start();
      click();
    }
  }

}
