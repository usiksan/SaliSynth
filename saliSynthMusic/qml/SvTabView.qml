/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Табулятор со страницами
  Каждая страница должна быть представлена SvTabViewItem
*/

import QtQuick 2.8
import QtQml.Models 2.2
import QtQuick.Layouts 1.3

//экран со вкладками
Item {
  anchors.fill: parent
  property ObjectModel tabs
  //ориентация содержимого - в каком направлении прокручиваются экраны
  property alias orientation: content.orientation
  property alias currentIndex: content.currentIndex
//  implicitWidth: mainWindow.width
//  implicitHeight: mainWindow.height

  property bool  flickEnable : true

  //размер панели вкладки (сверху или снизу - высота, слева/справа - ширина)
  property int   tabPanelSize: 50

  //положение панели вкладок (Qt.AlignBottom, Qt.AlignTop, Qt.AlignLeft, Qt.AlignRight)
  property int   tabPanelPosition: Qt.AlignBottom
  readonly property bool tabPanelIsVertical: tabPanelPosition == Qt.AlignLeft || tabPanelPosition == Qt.AlignRight

  //Цветовое и другое оформление
  property color buttonBackDefault : svStyle.tabButtonBackDefault
  property color buttonBackPressed : svStyle.tabButtonBackPressed
  property color buttonBackCurrent : svStyle.tabButtonBackCurrent
  property color buttonBorder      : svStyle.tabButtonBorder
  property int   buttonBorderWidth : svStyle.tabButtonBorderWidth
  property color textDefault       : svStyle.tabTextDefault
  property color textCurrent       : svStyle.tabTextCurrent
  property int   textSize          : svStyle.tabTextSize

  onTabPanelPositionChanged: updateTabPanelPosition()

  Component.onCompleted: {
    updateTabSizes()
    updateTabPanelPosition()
  }

  function setCurrentTab( idx ) {
    content.currentIndex = idx;
  }

  function updateTabSizes() {
    //изменить размер содержимого чтобы он соответствовал области содержимого
    for( var i=0; i<tabs.count; ++i ) {
      var obj = tabs.get(i)
      obj.parent = content;
      obj.width = Qt.binding(function() { return content.width })
      obj.height = Qt.binding(function() { return content.height })
      }
    }

  function updateTabPanelPosition() {
    //перемещение панели вкладок
    switch( tabPanelPosition ) {
      case Qt.AlignTop:
        content.Layout.row = 1
        content.Layout.column = 0
        tabPanel.Layout.row = 0
        tabPanel.Layout.column = 0
        tabPanel.Layout.fillWidth = true
        tabPanel.Layout.fillHeight = false
        break;
      case Qt.AlignBottom:
        content.Layout.row = 0
        content.Layout.column = 0
        tabPanel.Layout.row = 1
        tabPanel.Layout.column = 0
        tabPanel.Layout.fillWidth = true
        tabPanel.Layout.fillHeight = false
        break;
      case Qt.AlignLeft:
        content.Layout.row = 0
        content.Layout.column = 1
        tabPanel.Layout.row = 0
        tabPanel.Layout.column = 0
        tabPanel.Layout.fillWidth = false
        tabPanel.Layout.fillHeight = true
        break;
      case Qt.AlignRight:
        content.Layout.row = 0
        content.Layout.column = 0
        tabPanel.Layout.row = 0
        tabPanel.Layout.column = 1
        tabPanel.Layout.fillWidth = false
        tabPanel.Layout.fillHeight = true
        break;
      }
  }

  //Размещение страниц просмотра и табуляторов выбора страниц
  GridLayout {
    anchors.fill: parent
    rowSpacing: 0
    columnSpacing: 0

    //область содержимого
    StackLayout {
      id: content

      Layout.fillWidth: true
      Layout.fillHeight: true

      //property int currentIndex : 0
      property int orientation : 0
      clip: true
      //model: tabs
    }

    //Панель вкладок
    Flow {
      id: tabPanel
      Layout.fillWidth: true
      Layout.fillHeight: false
      flow:  tabPanelPosition == Qt.AlignTop || tabPanelPosition == Qt.AlignBottom ? Flow.LeftToRight : Flow.TopToBottom
      spacing: 0
      Repeater {
        model: tabs.count
        //Вкладка
        Item {
          id: tabItem
          property var tab: tabs.get(index)
          property bool isCurrent: content.currentIndex == index
          width: tabPanelIsVertical ? tabPanelSize : tabPanel.width/tabs.count - 0.01
          height: tabPanelIsVertical ? tabPanel.height/tabs.count : tabPanelSize

          //задник вкладки
          Rectangle {
            color: tabMouseArea.pressed ? buttonBackPressed : isCurrent ? buttonBackCurrent : buttonBackDefault
            border.width: isCurrent ? buttonBorderWidth : 0
            Behavior on border.width { NumberAnimation { } }
            border.color: "white"
            anchors.fill: parent
            anchors.bottomMargin:  tabPanelPosition == Qt.AlignBottom ? - 2 : 0
            anchors.topMargin:  tabPanelPosition == Qt.AlignTop ? - 2 : 0
            anchors.leftMargin:  tabPanelPosition == Qt.AlignLeft ? - 2 : 0
            anchors.rightMargin:  tabPanelPosition == Qt.AlignRight ? - 2 : 0
            Behavior on color { ColorAnimation { } }
            }

          //Содержимое вкладки: картинка и подпись
          ColumnLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 0
            //Картинка вкладки
            Image {
              visible: tab.tabImage != null
              source: tab.tabImage != null ? tab.tabImage: ""
              fillMode: Image.PreserveAspectFit
              Layout.fillWidth: true
              Layout.fillHeight: true
            }
            //Подпись вкладки
            Text {
              text: tab.tabName != null ? tab.tabName : ""
              Layout.fillWidth: true
              Layout.fillHeight: true
              wrapMode: Text.WrapAnywhere
              font.bold: tabItem.isCurrent
              font.pointSize: textSize
              color: tabItem.isCurrent ? textCurrent : textDefault
              Behavior on color { ColorAnimation { } }
              horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
            }
          }

          //Механизм выбора вкладки
          MouseArea {
            id: tabMouseArea
            anchors.fill: parent
            onClicked:  setCurrentTab( index );
          }
        }
      }
    }
  }
}
