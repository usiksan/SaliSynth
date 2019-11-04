/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Отладчик, который позволяет просматривать произвольные переменные
*/
import QtQuick 2.8
import SvQml 1.0

Rectangle {
  height: 500
  width: 300

  Column {
    anchors.fill: parent

    Repeater {
      model: 18
      Item {
        height: 25
        width: 370

        SvQmlValue { id: vgVar; mirror: vpu; }

        //Имя переменной
        SvFieldText {
          height: 25
          width: 150
          text: vgVar.name
          onApply: vgVar.name = str;
        }

        //Индекс переменной
        SvFieldNumber {
          height: 25
          width: 60
          x: 160
          text: vgVar.arIndex
          onApply: vgVar.arIndex = str;
        }

        //Значение переменной
        SvFieldVpuValue {
          height: 25
          x: 230
          width: 120
          value: vgVar
        }
      }
    }
  }
}
