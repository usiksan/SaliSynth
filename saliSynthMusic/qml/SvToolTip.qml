/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Всплывающая подсказка
*/
import QtQuick 2.0

MouseArea {
  anchors.fill: parent
  propagateComposedEvents: true
  hoverEnabled: true

  property alias delay : overTimer.interval
  Timer {
    id: overTimer
  }
}
