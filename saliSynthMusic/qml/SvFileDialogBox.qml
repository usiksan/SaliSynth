/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Окно диалога выбора файлов

  Идея в передаче в качестве параметра функции, которая должна выполниться при
  нажатии пользователем на ту или иную кнопку
*/
import QtQuick 2.2
import Qt.labs.folderlistmodel 2.1

SvModalItem {
  id: fileDialog
  //Функция, которая будет выполнена при успешном выборе файла
  property var okFunction

  //Функция открывает диалог для сохранения файлов
  function openSave( strTitle, strFolder, arrayFilter, ok ) {
    idTitle.text         = strTitle;
    files.folder         = "file://" + strFolder;
    files.nameFilters    = arrayFilter;
    currentFile.text     = strFolder;
    okFunction           = ok;
    openButton.text      = qsTr("Сохранить");
    visible              = true;
  }

  //Функция открывает диалог для загрузки файлов
  function openLoad( strTitle, strFolder, arrayFilter, ok ) {
    idTitle.text         = strTitle;
    files.folder         = "file://" + strFolder;
    files.nameFilters    = arrayFilter;
    currentFile.text     = strFolder;
    okFunction           = ok;
    openButton.text      = qsTr("Открыть");
    visible              = true;
  }


  //Область диалога
  Rectangle {
    anchors.fill: parent
    border.color: svStyle.frontColor
    color: "gray"

    //Приглашение
    SvText {
      id: idTitle
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.top: parent.top
      anchors.topMargin: 5
      font.pixelSize: 18
      color: svStyle.frontColor
    }

    //Текущий выбранный файл
    Rectangle {
      id: currentFileOver
      anchors.top: idTitle.bottom
      anchors.topMargin: 5
      anchors.left: parent.left
      anchors.leftMargin: 5
      anchors.right: parent.right
      anchors.rightMargin: 5
      height: 35
      color: svStyle.buttonBackDefault
      border.color: svStyle.frontColor

      TextInput {
        id: currentFile
        anchors.fill: parent
        anchors.margins: 5
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 16
        color: svStyle.frontColor
      }
    }


    //Список файлов
    Rectangle {
      id: list
      anchors.top: currentFileOver.bottom
      anchors.topMargin: 10
      anchors.left: parent.left
      anchors.leftMargin: 5
      anchors.right: parent.right
      anchors.rightMargin: 5
      anchors.bottom: buttons.top
      anchors.bottomMargin: 5
      border.color: svStyle.frontColor
      color: "transparent"

      FolderListModel {
        id: files
        //folder: "file://" + compLib.path;
        nameFilters: [ "*.kar" ]
        showDirs: true
        showDirsFirst: true
        showDotAndDotDot: true
        sortField: FolderListModel.Name
        sortReversed: false
      }

      Component {
        id: fileDelegate

        Rectangle {
          id: wrapper
          width: list.width - 30
          height: 30
          color: "transparent"

          //Иконка, которая показывает каталог или файл
          Image {
            width: 25
            height: 25
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            source: fileIsDir ? "qrc:/img/iconDir.png" : "qrc:/img/iconFile.png"
            fillMode: Image.PreserveAspectFit
          }

          //Имя файла или каталога
          SvText {
            id: nameText
            anchors.fill: parent; verticalAlignment: Text.AlignVCenter
            text: fileName
            anchors.leftMargin: 40
            font.pixelSize: 13
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
            color: svStyle.frontColor
          }

          MouseArea {
            id: mouseRegion
            anchors.fill: parent
            onDoubleClicked: {
              if( fileIsDir ) {
                //Войти в поддиректорий
                if( fileName === ".." )
                  //Выйти в родительский
                  files.folder = files.parentFolder
                else
                  files.folder = "file://" + filePath
                wrapper.ListView.view.currentIndex = -1;
              }
              else {
                okFunction(filePath)
                fileDialog.visible = false
              }
            }
            onClicked: {
              currentFile.text = filePath
              wrapper.ListView.view.currentIndex = index;
            }
          }
        }
      }

      SvScrollView {
        anchors.fill: parent

        ListView {
          id: view
          anchors.fill: parent
          anchors.margins: 10
          model: files
          delegate: fileDelegate
          focus: true
          highlight: Rectangle {
            color: svStyle.buttonBackDefault
            width: view.width - 10
            border.color: svStyle.frontColor
          }
          highlightMoveVelocity: -1
          pressDelay: 100
          clip: true
        }

      }
    }

    //Кнопки
    Row {
      id: buttons
      anchors.right: parent.right
      anchors.rightMargin: 5
      anchors.bottom: parent.bottom
      anchors.bottomMargin: 5

      height: 35
      spacing: 10

      //Кнопка Сохранить/Открыть
      SvButtonWithText {
        id: openButton
        onClick: {
          okFunction(currentFile.text)
          fileDialog.visible = false
        }
      }

      //Кнопка Отмена
      SvButtonWithText {
        text: qsTr("Отмена")
        onClick: fileDialog.visible = false
      }
    }

  }

}

