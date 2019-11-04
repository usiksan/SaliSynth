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
  property string imgSource : "qrc:/img/Kbd64"
  width: 64
  height: 64

  //Картинки для различных состояний кнопки
  property string imgDefault : imgSource + "Default.png"
  property string imgDisable : imgSource + "Disable.png"
  property string imgPressed : imgSource + "Pressed.png"

  //Цвета текста
  property color  textDefault: svStyle.buttonTextDefault
  property color  textDisable: svStyle.buttonTextDisable
  property color  textPressed: svStyle.buttonTextPressed
  property int    textSize: svStyle.buttonTextSize

  //Разрешение кнопки
  property bool   enable: true
  //Кнопка нажата
  property bool   pressed: false

  property string keyLow
  property string keyHigh
  property string engLow
  property string engHigh
  property bool   shift: false
  property bool   eng: false
  property bool   caps: false

  //Сигнал при нажатии на кнопку
  signal click( string key )

  //Картинка для кнопки
  source: enable ? (pressed ? imgPressed : imgDefault) : imgDisable

  //Основной текст
  SvText {
    id: butContens
    font.pointSize: 20
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.margins: 8
    text: eng ? ((caps && shift) || (!caps && !shift) ? engLow : engHigh) : ((caps && shift) || (!caps && !shift) ? keyLow : keyHigh )
    color: enable ? (pressed ? textPressed : textDefault) : textDisable
  }

  //Дополнительный текст
  SvText {
    font.pointSize: 11
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.margins: 10
    text: eng ? keyHigh : engHigh
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
      click( butContens.text );
    }
  }

}

