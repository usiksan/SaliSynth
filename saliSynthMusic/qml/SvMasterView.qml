/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Мастер со страницами
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

  //размер панели вкладки (сверху или снизу - высота, слева/справа - ширина)
  property int tabPanelSize: 50

  //Цветовое и другое оформление
  property color buttonBackDefault : svStyle.tabButtonBackDefault
  property color buttonBackPressed : svStyle.tabButtonBackPressed
  property color buttonBackCurrent : svStyle.tabButtonBackCurrent
  property color buttonBorder      : svStyle.tabButtonBorder
  property int   buttonBorderWidth : svStyle.tabButtonBorderWidth
  property color textDefault       : svStyle.tabTextDefault
  property color textCurrent       : svStyle.tabTextCurrent
  property int   textSize          : svStyle.tabTextSize

  Component.onCompleted: {
    updateTabSizes()
  }


  //Сигнал вызывается при нажатии на кнопку Назад
  signal prevPage();
  onPrevPage: {
    currentIndex = currentIndex - 1;
    var obj = tabs.get(currentIndex);
    //Выполнить функцию
    if( obj.enterFunctionBackward.lenght )
      vpu.vpuExecuteFunction0( obj.enterFunctionBackward );
  }



  //Сигнал вызывается при нажатии на кнопку Далее
  signal nextPage();
  onNextPage: {
    var curObj = tabs.get(currentIndex);
    //Проверить возможность перехода
    curObj.testNextEnable();
    if( curObj.isNextEnable ) {
      //Переход возможен
      //Если дошли до последней страницы, то выход
      if( currentIndex + 1 >= tabs.count ) {
        exitPage();
        currentIndex = 0;
      }
      else {
        currentIndex = currentIndex + 1;
        var obj = tabs.get(currentIndex);
        //Выполнить функцию
        if( obj.enterFunctionForward.length )
          vpu.vpuExecuteFunction0( obj.enterFunctionForward );
      }
    }

  }



  //Сигнал вызывается при нажатии на кнопку Выход
  signal exitPage();

  function updateTabSizes() {
    //изменить размер содержимого чтобы он соответствовал области содержимого
    for( var i=0; i < tabs.count; ++i ) {
      var obj = tabs.get(i)
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

  //область содержимого
  ListView {
    id: content

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.bottomMargin: tabPanelSize

    orientation: ListView.Horizontal
    snapMode: ListView.SnapOneItem
    boundsBehavior: Flickable.StopAtBounds
    highlightRangeMode: ListView.StrictlyEnforceRange

    maximumFlickVelocity: width * 2

    highlightFollowsCurrentItem: true
    highlightMoveDuration: 500
    clip: true
    model: tabs
  }

  //Панель с кнопками перехода
  Item {
    id: tabPanel
    height: tabPanelSize
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom

    //Слева кнопка Назад
    SvButtonImage {
      imgDefault: "qrc:/img/arrowLeft.png"
      imgDisable: imgDefault
      imgPressed: imgDefault
      text: qsTr("Назад")
      enable: content.currentIndex > 0 && !machineIsExecutingFunction
      anchors.left: parent.left
      anchors.leftMargin: 5
      anchors.top: parent.top
      onClick: prevPage();
    }

    //По центру кнопка Далее
    SvButtonImage {
      imgDefault: "qrc:/img/arrowRight.png"
      imgDisable: imgDefault
      imgPressed: imgDefault
      text: qsTr("Далее")
      enable: !machineIsExecutingFunction
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.top: parent.top
      onClick: nextPage();
    }

    //Справа кнопка Выход
    SvButtonImage {
      imgSource: "qrc:/img/buttonExit"
      height: tabPanelSize
      width: tabPanelSize
      anchors.right: parent.right
      anchors.rightMargin: 5
      anchors.top: parent.top
      onClick: exitPage();
      enable: !machineIsExecutingFunction
    }
  }

}
