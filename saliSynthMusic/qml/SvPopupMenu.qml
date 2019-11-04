/*
Project "Установщик B800"

Author
  Sibilev Alexander S.

Description
  Всплывающее меню общего назначения

  Показывает меню возле выбранной точки.
*/
import QtQuick 2.8
import SvQml 1.0

Item {
  id: popupMenu

  anchors.fill: parent

  visible: false

  property int menuCount: 0 //Количество элементов массива
  property int menuCurrent : -1
  property var menuText    : ["def"] //Массив пунктов
  property var menuFunction : [null] //Массив функций-реакций на выбор пункта

  property int menuWidth : 200
  property int menuItemHeight : 25


  function showMenu( point, count, menu ) {
    //Выделить новые массивы для меню
    menuText = new Array(count);
    menuFunction = new Array(count);
    menuCount = 0;
    //Заполнить массивы меню
    var i;
    for( i = 0; i < count; i = i + 1 ) {
      menuText[i] = menu[i*2];
      menuFunction[i] = menu[i*2+1];
    }

    menuCount = count;
    menuCurrent = 0;

    //Размещаем меню по умолчанию внизу и вправо
    var pt = mapFromGlobal( point.x, point.y )
    if( pt.x + menuWidth > width )
      //Выпирает за правую границу, делаем влево
      pt.x = width - menuWidth
    if( pt.y + count * menuItemHeight > height )
      //Выпирает вниз, делаем вверх
      pt.y = pt.y - count * menuItemHeight;
    menuBox.x = pt.x;
    menuBox.y = pt.y;

    //Показать меню
    popupMenu.visible = true;
  }

  //Область, которая закрывает все внизу и при нажатии убирает меню
  MouseArea {
    anchors.fill: parent
    onClicked: popupMenu.visible = false
  }

  //Размещение меню
  Rectangle {
    id: menuBox
    width: menuWidth
    height: menuItemHeight * menuCount
    color: svStyle.buttonBackDisable
    border.color: svStyle.buttonBorderDisable

    Column {
      Repeater {
        model: menuCount

        Item {
          width: menuWidth
          height: menuItemHeight

          //Прямоугольник для отметки текущего пункта меню
          Rectangle {
            anchors.fill: parent
            color: svStyle.buttonBackPressed
            visible: index === menuCurrent
          }

          //Подпись для пункта меню
          SvText {
            anchors.fill: parent
            anchors.margins: 2
            clip: true
            text: menuText[index]
            color: svStyle.buttonTextDefault
          }
        }
      }
    }
  }

  //Отслеживание положения мыши на пунктах
  MouseArea {
    anchors.fill: menuBox
    hoverEnabled: true; //Разрешить отслеживание курсора
    //При наведении курсора сделать данный пункт текущим
    onPositionChanged: menuCurrent = mouseY / menuItemHeight
    onClicked: {
      //Выключить меню
      popupMenu.visible = false;
      //Выполнить функцию, ассоциированную с пунктом
      if( menuFunction[menuCurrent] !== null )
        menuFunction[menuCurrent]();
    }
  }

}
