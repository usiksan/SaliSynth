/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Элемент всплывающей подсказки

  Есть центральный объект svHelpSystem со свойством toolTipEnable. Когда оно активируется элемент воспринимает
  нажатие мышью и показывает всплывающую подсказку.

  Всплывающая подсказка выполняется через объект svHelpSystem типа SvHelpSystem
*/
import QtQuick 2.0

MouseArea {
  id: helpItem
  anchors.fill: parent
  enabled: svHelpSystem.toolTipEnable
  z: 1 //Чтобы область была поверх остальных объектов

  property string toolTip
  property string helpSection

  onClicked: svHelpSystem.toolTip( helpItem, toolTip, helpSection )
}
