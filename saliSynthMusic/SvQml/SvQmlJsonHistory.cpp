/*
Project "Интерфейс специальных инструментов с QML"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  QML-интерфейс к истории, выраженной текстом.
  Может работать в качестве простого списка строк, а может и с "перестановкой" в зависимости от частоты использования.
  Основное назначение - история последних открытых файлов.
  Поскольку наследуется JSON-значение, то этот список автоматически сохраняется в файле json
*/
#include "SvQmlJsonHistory.h"

SvQmlJsonHistory::SvQmlJsonHistory(QObject *parent) :
  SvQmlJsonValue(parent),
  mLimit(6) //Ограничение количества строк
  {

  }




//Доступ к ограничителю количества строк
void SvQmlJsonHistory::setLimit(int lim)
  {
  mLimit = lim;
  while( mLimit > mList.count() )
    mList.removeLast();
  emit limitChanged();
  //Обновить в json
  updateToJson();
  }




//Очистить список
void SvQmlJsonHistory::clear()
  {
  mList.clear();
  //Обновить в json
  updateToJson();
  }




//Добавить строку в начало списка
void SvQmlJsonHistory::push(const QString str)
  {
  //Добавить в начало
  mList.push_front( str );
  //Если количество превысило лимит, то удалить последний
  if( mList.count() > mLimit )
    mList.pop_back();
  //Обновить в json
  updateToJson();
  }




//Добавить строку в конец списка
void SvQmlJsonHistory::append(const QString str)
  {
  //Добавить в конец
  mList.push_back( str );
  //Если количество превысило лимит, то удалить первый
  if( mList.count() > mLimit )
    mList.pop_front();
  //Обновить в json
  updateToJson();
  }




//Добавить уникальную строку
void SvQmlJsonHistory::insertUnical(const QString str)
  {
  //Удалить из списка все строки идентичные вставляемой
  mList.removeAll( str );
  //Вставить строку в начало списка
  push( str );
  }




//Вызывается из JSON после загрузки объекта, чтобы скорректировать значение в соответствии с загруженным
void SvQmlJsonHistory::onLoadJson()
  {
  if( mFile != nullptr && !mJsonName.isEmpty() ) {
    mList = mFile->asStringList( mJsonName );
    emit listChanged();
    }
  }




//Вызывается из JSON перед сохранением объекта, чтобы обновить в модели файла все значения
void SvQmlJsonHistory::onSaveJson()
  {
  if( mFile != nullptr && !mJsonName.isEmpty() )
    mFile->setStringList( mJsonName, mList );
  }




//Обновить список в json
void SvQmlJsonHistory::updateToJson()
  {
  if( mFile != nullptr )
    mFile->setDirty();
  emit listChanged();
  }
