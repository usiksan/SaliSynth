/*
Project "Принтер b073-av-af-as-ac"

Author
  Sibilev Alexander S.

Description
  Стиль приложения
*/
import QtQuick 2.8

Item {
  //Цвет элементов
  property color  frontColor: "black"
  property color  backColor: "white"

  //Табулятор
  property color  tabButtonBackDefault : backColor
  property color  tabButtonBackPressed : Qt.darker(tabButtonBackDefault)
  property color  tabButtonBackCurrent : "black"

  property color  tabButtonBorder      : "white"
  property int    tabButtonBorderWidth : 2

  property color  tabTextDefault       : frontColor
  property color  tabTextCurrent       : "white"
  property int    tabTextSize          : 12


  //Поля
  //Свойства по умолчанию для названия поля
  property color  fieldName            : frontColor
  property color  fieldNameDisable     : Qt.darker(fieldName)
  property int    fieldNameSize        : 14
  //Свойства по умолчанию для самого поля
  property color  fieldDefault         : frontColor
  property color  fieldDisable         : Qt.darker(fieldDefault)
  property int    fieldSize            : 16
  //Свойства по умолчанию для индикаторов
  property color  fieldIndicatorActive : "green"
  property color  fieldIndicatorPassive: "transparent"


  //Всплывающие подсказки
  property color  toolTipBack          : "yellow"
  property color  toolTipBorder        : "black"
  property color  toolTipText          : "black"
  property int    toolTipTextSize      : 12

  //цвет текста, который можно редактировать
  property color editTextColor: frontColor
  //показывать ли задник поля редактирования
  property bool  editShowBackground: false
  //цвет задника поля редактирования
  property color editBackgroundColor: "white"

  //главный шрифт для приложения (преимущественно не используется)
  property string mainFontFamily: "Arial"

  //цвета кнопок (преимущественно не используется)
  property string buttonFontFamily: mainFontFamily
  property int    buttonTextSize: 12
  property color  buttonNormalColor: frontColor
  property color  buttonPressedColor: Qt.darker(buttonNormalColor)
  property color  buttonDisabledColor: Qt.tint(buttonNormalColor, "darkgray")
  property color  buttonHoverColor: Qt.lighter(buttonNormalColor)
  property color  buttonCheckedColor: Qt.lighter("green")
  property color  buttonHighlightColor: Qt.lighter(Qt.lighter(buttonNormalColor))

  //Цвета прямоугольных кнопок
  property color  buttonBackDefault: backColor
  property color  buttonBackDisable: Qt.darker(buttonBackDefault)
  property color  buttonBackPressed: Qt.lighter(buttonBackDefault)
  property color  buttonBorderDefault: frontColor
  property color  buttonBorderDisable: Qt.darker(buttonBorderDefault)
  property color  buttonBorderPressed: Qt.lighter(buttonBorderDefault)
  property color  buttonTextDefault: frontColor
  property color  buttonTextDisable: Qt.darker(buttonTextDefault)
  property color  buttonTextPressed: Qt.lighter(buttonTextDefault)

  //Цвета окна сообщений
  property color  messageBoxBack  : Qt.darker( backColor )
  property color  messageBoxBorder: frontColor
  property color  messageBoxTitle: frontColor
  property int    messageBoxTitleSize: 14
  property color  messageBoxMsg: frontColor
  property int    messageBoxMsgSize: 16

  //Цвета окна ввода цифровых значений
  //property color  title: value

  //Цвета виртуальной клавиатуры
  property color  keyboardBack : backColor
  property color  keyboardEditBack: Qt.lighter(keyboardBack)
  property color  keyboardEditText: frontColor

  //цвета спиннера
  property color spinnerBackgroundColor: "white"
  property color spinnerTextColor: "black"
  property color spinnerIndicatorColor:  "#559cf2f2"
  property color spinnerBorderColor: "steelblue"

  //Цвет выбранного питателя
  property color colorSelectedFeederTrack : "yellow"
  //Цвет пустой дорожки
  property color colorEmptyTrack : "#106687"
  //Цвет занятой дорожки
  property color colorBusyTrack : Qt.lighter("green")

}
