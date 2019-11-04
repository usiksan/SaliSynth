/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Кнопка на основе картинки с подписью по картинке
*/
import QtQuick 2.8

Image {
  //Базовая картинка для всех кнопок
  property string imgSource
  //Картинки для различных состояний кнопки
  property string imgDefault : imgSource + "Default.png"
  property string imgDisable : imgSource + "Disable.png"
  property string imgPressed : imgSource + "Pressed.png"

  //Цвета текста
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

  //Сигналы при нажатии и при отпускании
  signal press()
  signal release()

  //Картинка для кнопки
  source: enable ? (pressed ? imgPressed : imgDefault) : imgDisable

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
      press()
    }
    onReleased: release()
  }

}
