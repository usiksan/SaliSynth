/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Настроенный стандартный элемент выбора из списка
*/
import QtQuick 2.8
import QtQuick.Controls 2.0

ComboBox {
  id: control
  implicitHeight: 30

  delegate: ItemDelegate {
    width: control.width
    contentItem: Text {
      text: modelData
      color: svStyle.buttonTextDefault
      font.family: svStyle.buttonFontFamily
      font.pixelSize: svStyle.buttonTextSize
      elide: Text.ElideRight
      verticalAlignment: Text.AlignVCenter
      }
    highlighted: control.highlightedIndex === index
    background: Rectangle {
      color: control.highlightedIndex === index ? svStyle.buttonBackPressed : svStyle.buttonBackDefault
    }
  }

  indicator: Canvas {
    id: canvas
    x: control.width - width - control.rightPadding
    y: control.topPadding + (control.availableHeight - height) / 2
    width: 12
    height: 8
    contextType: "2d"

    Connections {
      target: control
      onPressedChanged: canvas.requestPaint()
      }

    onPaint: {
      context.reset();
      context.moveTo(0, 0);
      context.lineTo(width, 0);
      context.lineTo(width / 2, height);
      context.closePath();
      context.fillStyle = control.pressed ? svStyle.buttonTextPressed : svStyle.buttonTextDefault;
      context.fill();
    }
  }

  contentItem: Text {
    leftPadding: 2
    rightPadding: control.indicator.width + control.spacing

    text: control.displayText
    font.family: svStyle.buttonFontFamily
    font.pixelSize: svStyle.buttonTextSize
    color: control.pressed ? svStyle.buttonTextPressed : svStyle.buttonTextDefault
    horizontalAlignment: Text.AlignLeft
    verticalAlignment: Text.AlignVCenter
    elide: Text.ElideRight
  }

  background: Rectangle {
    implicitWidth: 150
    implicitHeight: 40
    color: svStyle.buttonBackDefault
    border.color: control.pressed ? svStyle.buttonBorderPressed : svStyle.buttonBorderDefault
    border.width: control.visualFocus ? 2 : 1
    radius: 2
    }

  popup: Popup {
    y: control.height - 1
    width: control.width
    implicitHeight: contentItem.implicitHeight
    padding: 2

    contentItem: ListView {
      clip: true
      implicitHeight: contentHeight
      model: control.popup.visible ? control.delegateModel : null
      currentIndex: control.highlightedIndex

      ScrollIndicator.vertical: ScrollIndicator { }
      }

    background: Rectangle {
      color: svStyle.buttonBackDefault
      border.color: svStyle.buttonBorderDefault
      radius: 2
      }
   }

}
