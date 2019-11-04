/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Элемент всплывающей подсказки

  Есть центральный объект svHelpSystem со свойством toolTipEnable. Когда оно активируется элемент воспринимает
  нажатие мышью и показывает всплывающую подсказку.

  Всплывающая подсказка выполняется через объект svHelpSystem типа SvHelpSystem, поэтому
  SvHelpSystem должен быть объявлен в корневом элементе и точно с именем svHelpSystem,
  чтобы быть видимым для отдельных элементов
*/
import QtQuick 2.0

Item {
  id: helpSystem
  anchors.fill: parent
  visible: false

  //Активация отдельных элементов помощи
  //Устанавливается в true внешней системой
  property bool toolTipEnable : false

  //Время активного состояния Активации помощи
  property alias toolTipEnableDuration : idToolTipEnableDuration.duration

  //Время, в течение которого отображается подсказка
  property int showTime : 10

  //Секция основной помощи
  property string mainHelpSection

  signal showHelpSection( string helpSection );

  property string helpPage

  //Автоматическое выключение активации отдельных элементов помощи
  PauseAnimation {
    id: idToolTipEnableDuration
    duration: 3000
    onStopped: toolTipEnable = false;
  }

  //Функция разрешает или запрещает активацию отдельных элементов помощи
  function toggleToolTipEnable() {
    if( toolTipEnable )
      toolTipEnable = false;
    else {
      //Если шел показ другой подсказки, то остановить его
      showAnimation.stop();
      toolTipEnable = true;
      idToolTipEnableDuration.start();
    }
  }

  //Функция, осуществляющая вывод подсказки
  function toolTip( helpItem, toolTipText, helpSection ) {
    //Назначить текст
    idToolTipText.text = toolTipText
    mainHelpSection = helpSection

    //Разрешить переход к секции помощи, если она назначена
    if( mainHelpSection.length > 0 )
      idToolTipPress.enabled = true;



    //Если ширина по умолчанию слишком большая, то ограничить ее размером содержимого
    if( idToolTipText.contentWidth > helpSystem.width / 2.5 )
      idToolTipText.width = helpSystem.width / 2.5
    else
      idToolTipText.width = idToolTipText.contentWidth + 10

    //Высоту установить в соответствии с содержимым
    idToolTipText.height = idToolTipText.contentHeight + 10



    //Определить положение элемента
    var pos = mapFromItem( helpItem, 0, 0, helpItem.width, helpItem.height );



    //Спозиционировать подсказку по X
    if( pos.x > idToolTipBack.width )
      //Подсказка влезает с левой стороны
      idToolTipText.x = pos.x - idToolTipBack.width - 3;
    else if( (pos.x + pos.width) < (helpSystem.width - idToolTipBack.width) )
      //Подсказка влезает с правой стороны
      idToolTipText.x = pos.x + pos.width + 3;
    else
      //Разместить подсказку по центру
      idToolTipText.x = pos.x + (pos.width / 2) - (idToolTipBack.width / 2);

    //Спозиционировать подсказку по Y
    if( (pos.y + pos.height) < (helpSystem.height - idToolTipBack.height) )
      //Подсказка влезает снизу
      idToolTipText.y = pos.y + pos.height + 3;
    else if( pos.y > idToolTipBack.height )
      //Подсказка влезает сверху
      idToolTipText.y = pos.y - idToolTipBack.height - 3;
    else
      //Разместить подсказку по центру
      idToolTipText.y = pos.y + (pos.height / 2) - (idToolTipBack.height / 2);

    //Выключить ожидание Активации
    idToolTipEnableDuration.stop();
    //Все готово, показать подсказку
    showAnimation.start();
  }

  SequentialAnimation {
    id: showAnimation

    //Показать
    NumberAnimation {
      targets: [idToolTipBack,idToolTipText]
      properties: "opacity"
      duration: 400
      from: 0.0
      to: 1.0
    }

    //Отображаем подсказку
    PauseAnimation {duration: showTime * 1000}

    //Убираем подсказку
    NumberAnimation {
      targets: [idToolTipBack,idToolTipText]
      properties: "opacity"
      duration: 400
      from: 1.0
      to: 0.0
    }

    onStopped: {
      //При остановке анимации погасить подсказку
      helpSystem.visible = false;
    }

    onStarted: {
      idToolTipBack.opacity = 0;
      idToolTipText.opacity = 0;
      idToolTipPress.enabled = true;
      helpSystem.visible = true;
    }
  }

  //Область с подсказкой
  Rectangle {
    id: idToolTipBack
    anchors.fill: idToolTipText
    anchors.margins: -4

    color: svStyle.toolTipBack
    border.color: svStyle.toolTipBorder
    border.width: 1
  }

  //Текст с подсказкой
  SvText {
    id: idToolTipText
    //Обеспечим заворачивание текста по ширине
    wrapMode: Text.WordWrap

    width: parent.width / 2.5
  }

  //Область нажатия для показа дополнительной помощи
  MouseArea {
    id: idToolTipPress
    anchors.fill: idToolTipBack
    onClicked: {
      //Прекратить показ подсказки
      showAnimation.stop();
      //Вызвать секцию помощи
      showHelpSection( mainHelpSection )
    }
  }
}
