/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Прокручиваемый элемент с полосой прокрутки.
*/
import QtQuick 2.8
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3


ScrollView {

  style: ScrollViewStyle {
    scrollBarBackground: Item{ width: 15 }
    incrementControl: Item {width: 10; height: 10}
    decrementControl: Item {width: 10; height: 10}
    corner: null

    handle: Rectangle {
      implicitWidth: 10
      radius: 5
      color: Qt.darker('blue')
    }
  }
}

