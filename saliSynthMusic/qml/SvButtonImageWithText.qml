/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Кнопка с картинкой и подписью
*/
import QtQuick 2.8
import QtQuick.Layouts 1.3

Item {
  id: but
  property string img: ""
  property string imgDefault: img + "Default.png" //Название картинки для состояния по умолчанию
  property string imgDisable: utils.pathDefault( img + "Disable.png", imgDefault ) //Название картинки для состояния Запрещена
  property string imgPressed: utils.pathDefault( img + "Pressed.png", imgDefault ) //Название картинки для состояния Нажата

  //Цвета текста
  property color  textDefault: svStyle.buttonNormalColor
  property color  textDisable: svStyle.buttonDisabledColor
  property color  textPressed: svStyle.buttonPressedColor

  //Размер текста
  property int    textSize: svStyle.buttonTextSize

  property alias  legend: textItem.text
  property bool   enable: true
  property bool   pressed: false

  //Отсутпы текста от картинки
  property int    textMargin  : 5

  //размеры кнопки. -1 - нет ограничения размеров
  property int    buttonWidth : -1
  property int    buttonHeight: -1

  //считаются желаемые размеры текста и кнопки, которые будут использованы для расчета размера элемента в целом
  property int buttonDesiredHeight: buttonHeight > 0 ? buttonHeight : buttonItem.implicitHeight
  property int buttonDesiredWidth: (buttonWidth > 0 ? buttonWidth : buttonItem.implicitWidth )

  //Масштабный коэффициент для заданный размеров картинки
  property real buttonScaleWidth : buttonDesiredWidth / buttonItem.implicitWidth
  property real buttonScaleHeight: buttonDesiredHeight / buttonItem.implicitHeight

  signal clicked

  implicitWidth : Math.max( buttonDesiredWidth, textItem.implicitWidth )
  implicitHeight: buttonDesiredHeight + textItem.implicitHeight + textMargin


  Rectangle {
    color: "transparent"
    border.color: textDefault
    anchors.fill: contens
    anchors.margins: -8 //Увеличить размер охватывающего прямоугольника
    opacity: pressed ? 1.0 : 0
  }

  ColumnLayout {
    id: contens
    spacing: textMargin

    //Картинка для состояний кнопки
    Image {
      id: buttonItem
      anchors.horizontalCenter: parent.horizontalCenter
      source: but.pressed ? imgPressed : (but.disable ? imgDisable : imgDefault)
      Layout.preferredWidth: buttonDesiredWidth
      Layout.preferredHeight: buttonDesiredHeight
    }


    //Подпись
    SvText {
      id: textItem
      anchors.horizontalCenter: parent.horizontalCenter
      font.family: svStyle.buttonFontFamily
      font.pixelSize: textSize
      horizontalAlignment: Text.AlignHCenter
      width: parent.width
      color: but.pressed ? textPressed : (but.enable ? textDefault : textDisable)
      wrapMode: Text.WordWrap
    }
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
    enabled: but.enable
    onPressed: {
      pressAnim.start();
      but.clicked();
    }
  }

}
