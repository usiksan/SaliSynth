/*
Project "Библиотека общих элементов управления"

Author
  Sibilev Alexander S.

Description
  Элемент контекстной помощи

  Этот элемент вставляется в страницы режимов. Поэтому активная в настоящий момент страница откликается на вызов помощи.
*/
import QtQuick 2.8

Item {
  property string helpPage
  property string prevHelpPage
  onVisibleChanged: {
    if( visible ) {
      prevHelpPage = svHelpSystem.helpPage
      svHelpSystem.helpPage = helpPage
    }
    else if( svHelpSystem !== null ) {
      if( svHelpSystem.helpPage === helpPage )
        svHelpSystem.helpPage = prevHelpPage
    }
//    console.log( "prevHelpPage", prevHelpPage );
//    console.log( "svHelpSystem.helpPage", svHelpSystem.helpPage )
  }
}
