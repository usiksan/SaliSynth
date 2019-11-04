/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Модальный элемент. Блокирует весь экран кроме своей области ввода.
*/
import QtQuick 2.8

Item {
  //Заполняем все пространство
  anchors.fill: parent

  //По умолчанию выключен
  visible: false

  //Сигнал, вызывается при нажатии вне области интереса
  signal outLineClick

  //Область используемая для затемнения изображения экрана
  Rectangle {
    //Цвет используется для затемнения остального изображения
    color: "gray"

    //Прозрачность используется для затемнения
    opacity: 0.5

    //Занимает всю область
    anchors.fill: parent
  }

  //Здесь ловим все левые нажатия
  MouseArea {
    anchors.fill: parent
    onClicked: outLineClick();
  }

}
