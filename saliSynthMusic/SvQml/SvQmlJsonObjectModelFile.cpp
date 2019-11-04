/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SvQmlJsonObjectModelFile.h"
#include "SvDir.h"
#include "SvQmlUtils.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <QDebug>
#include <QRegExp>

SvQmlJsonObjectModelFile::SvQmlJsonObjectModelFile(QObject *parent) :
  QAbstractListModel( parent ),
  mDirty(false),
  mLowerKey(true)
  {
  //По умолчанию - храним в домашнем каталоге
  mPath = SvQmlUtils::getHomePath();

  //Задаем интервал сканирования по умолчанию, но таймер не запускаем
  //т.е. по умолчанию автосохранение отключено
  mAutoSave.setInterval( 1000 );

  //Настроить реакцию таймера
  connect( &mAutoSave, &QTimer::timeout, this, [=] () {
    //Если грязный, то сохраняем
    if( mDirty )
      save();
    });
  }

//Доступ к списку полей
void SvQmlJsonObjectModelFile::setFields(QStringList f)
  {
  beginResetModel();
  mFields = f;
  endResetModel();
  emit fieldsChanged();
  }




//Доступ к имени файла
void SvQmlJsonObjectModelFile::setFile(const QString fl)
  {
  //Установить новое имя
  mFileName = fl;
  //Загрузить файл с новым именем
  load();
  }




//Доступ к пути для файла
void SvQmlJsonObjectModelFile::setPath(const QString pt)
  {
  SvDir dir( pt );
  mPath = dir.slashedPath();
  //Построить этот полный путь
  QDir( mPath ).mkpath( mPath );

  if( !mFileName.isEmpty() )
    //Загрузить файл с новым путем
    load();
  }




//Доступ к свойству автоматического сохранения
int SvQmlJsonObjectModelFile::autoSave() const
  {
  return mAutoSave.isActive() ? mAutoSave.interval() : 0;
  }




//Доступ к свойству автоматического сохранения
void SvQmlJsonObjectModelFile::setAutoSave(int period)
  {
  if( period ) {
    //Если ненулевой период, то установить период и запустить таймер
    mAutoSave.setInterval( period );
    mAutoSave.start();
    }
  else
    //Если период нулевой, то остановить таймер
    mAutoSave.stop();
  //Сигнал об измении системы автосохранения
  emit autoSaveChanged();
  }




//Доступ к фильтру
void SvQmlJsonObjectModelFile::setFilter(const QString f)
  {
  mFilter = f;
  emit filterChanged();
  refilter();
  }



//Доступ к флагу перевода ключей в нижний регистр
void SvQmlJsonObjectModelFile::setLowerKey(bool low)
  {
  mLowerKey = low;
  emit lowerKeyChanged();
  }




int SvQmlJsonObjectModelFile::rowCount(const QModelIndex &parent) const
  {
  Q_UNUSED(parent)
  return mFilterMap.count();
  }




//Данные из таблицы
QVariant SvQmlJsonObjectModelFile::data(const QModelIndex &index, int role) const
  {
  //Проверить допуск индексов
  if( !index.isValid() || index.row() >= mFilterMap.count() || index.row() < 0 || role <= Qt::UserRole )
    return QVariant();

  if( (role - Qt::UserRole - 1) == 0 )
    return mFilterMap.at( index.row() );
  QString nm = mFields.at( role - Qt::UserRole - 1 );
  return mContens.value( mFilterMap.at( index.row() ) ).toObject().value( nm ).toVariant();
  }




bool SvQmlJsonObjectModelFile::setData(const QModelIndex &index, const QVariant &value, int role)
  {
  //Проверить допуск индексов
  if( !index.isValid() || index.row() >= mFilterMap.count() || index.row() < 0 || role <= Qt::UserRole )
    return false;

  if( (role - Qt::UserRole - 1) == 0 )
    return false;
  QString nm = mFields.at( role - Qt::UserRole - 1 );
  setMappedString( index.row(), nm, value.toString() );
  return true;
  }




Qt::ItemFlags SvQmlJsonObjectModelFile::flags(const QModelIndex &index) const
  {
  Q_UNUSED( index )
  return Qt::ItemIsEditable | Qt::ItemIsEnabled;
  }





QHash<int, QByteArray> SvQmlJsonObjectModelFile::roleNames() const
  {
  QHash<int, QByteArray> roles;
  int i = 0;
  for( QString fn : mFields )
    roles.insert( Qt::UserRole + 1 + i++, fn.toUtf8() );
  return roles;
  }




//Сохранить JSON файл
bool SvQmlJsonObjectModelFile::save()
  {
  return saveTo( mPath + mFileName );
  }




//Сохранить JSON файл под другим именем
bool SvQmlJsonObjectModelFile::saveTo(const QString filePath)
  {
  //Если имя файла нулевое, то выход с ошибкой
  if( filePath.isEmpty() ) {
    emit saveComplete( false );
    return false;
    }

  //Чтобы выделить путь к файлу
  QFileInfo info( filePath );

  //Построить этот полный путь
  QDir dir( info.absoluteDir() );
  dir.mkpath( info.absolutePath() );

  //Открыть файл
  QFile file( filePath );
  if( file.open( QIODevice::WriteOnly) ) {
    QJsonDocument doc( mContens );
    file.write( doc.toJson() );
    file.close();
    emit saveComplete( true );
    mDirty = false;
    emit isDirtyChanged();
    return true;
    }
  emit saveComplete( false );
  return false;
  }




//Загрузить JSON файл
bool SvQmlJsonObjectModelFile::load()
  {
  return loadFrom( mPath + mFileName );
  }



//Загрузить JSON файл под другим именем
bool SvQmlJsonObjectModelFile::loadFrom(const QString filePath)
  {
  //Открыть файл
  QFile file( filePath );
  if( file.open( QIODevice::ReadOnly) ) {
    mContens = QJsonDocument::fromJson( file.readAll() ).object();
    file.close();

    mDirty = false;
    emit isDirtyChanged();

    //Отправить сигнал об успешной загрузке
    emit loadComplete( true );

    //Переформируем модель
    refilter();

    return true;
    }
  emit loadComplete( false );
  return false;
  }



bool SvQmlJsonObjectModelFile::contains(const QString key)
  {
  if( mLowerKey )
    return mContens.contains( key.toLower() );
  return mContens.contains(key);
  }



QString SvQmlJsonObjectModelFile::mappedKey(int row) const
  {
  if( row < 0 || row >= mFilterMap.count() )
    return QString();
  return mFilterMap.at(row);
  }




//Получить строку из головного объекта
QString SvQmlJsonObjectModelFile::asString(const QString key, const QString nm) const
  {
  QString lowKey = mLowerKey ? key.toLower() : key;
  if( mFields.count() <= 0 )
    return QString();
  if( mFields.at(0) == nm )
    return lowKey;
  if( mContens.contains(lowKey) )
    return mContens.value(lowKey).toObject().value(nm).toString();
  return QString();
  }




QString SvQmlJsonObjectModelFile::asMappedString(int row, const QString nm) const
  {
  //Проверить допуск индексов
  if( row < 0 || row >= mFilterMap.count() )
    return QString();
  return asString( mFilterMap.at(row), nm );
  }




//Установить строку в головной объект
void SvQmlJsonObjectModelFile::setString(const QString key, const QString nm, const QString val)
  {
  QString lowKey = mLowerKey ? key.toLower() : key;
  //Получить объект для модификации
  QJsonObject obj = mContens.value(lowKey).toObject();
  //Модифицировать объект
  obj.insert( nm, val );
  //Обновить по ключу
  mContens.insert( lowKey, obj );
  //Найти данный ключ в фильтре
  int row = mFilterMap.indexOf( lowKey );
  int column = mFields.indexOf( nm );
  if( row >= 0 && column > 0 )
    emit dataChanged( index(row), index(row), QVector<int>( 1, Qt::UserRole + 1 + column ) );
  //Данные изменились, сообщить, что файл грязный
  setDirty();
  }




void SvQmlJsonObjectModelFile::setMappedString(int row, const QString nm, const QString val)
  {
  //Проверить допуск индексов
  if( row < 0 || row >= mFilterMap.count() )
    setString( mFilterMap.at(row), nm, val );
  }



double SvQmlJsonObjectModelFile::asReal(const QString key, const QString nm) const
  {
  //Заменить запятые на точки
  QString str = asString( key, nm );
  str = str.replace( QChar(','), QString(".") );
  return str.toDouble();
  }



void SvQmlJsonObjectModelFile::setIntAsReal(const QString key, const QString nm, int val, int factor)
  {
  double v = val;
  double f = factor;
  setString( key, nm, QString::number( v / f, 'f', 3) );
  }



//Установить флаг грязности
void SvQmlJsonObjectModelFile::setDirty()
  {
  mDirty = true;
  emit isDirtyChanged();
  }




//Сбросить флаг грязности
void SvQmlJsonObjectModelFile::clearDirty()
  {
  mDirty = false;
  emit isDirtyChanged();
  }



//Добавить запись к таблице
bool SvQmlJsonObjectModelFile::addRecord(const QString key)
  {
  QString lowKey = mLowerKey ? key.toLower() : key;
  //Если ключ уже есть, то возврат
  if( mContens.contains(lowKey) )
    return false;
  beginInsertRows( QModelIndex(), mFilterMap.count(), mFilterMap.count() );
  QJsonObject obj;
  for( QString fn : mFields )
    obj.insert( fn, QJsonValue( QString("") ) );
  mFilterMap.append( lowKey );
  mContens.insert( lowKey, obj );
  //Данные изменились, сообщить, что файл грязный
  setDirty();
  endInsertRows();

  return true;
  }




//Удалить запись из модели
void SvQmlJsonObjectModelFile::removeRecord(const QString key)
  {
  QString kkey = mLowerKey ? key.toLower() : key;
  //Если ключа нету, то возврат
  if( !mContens.contains(kkey) )
    return;

  //Проверить, есть ли данная запись в таблице
  int row = mFilterMap.indexOf( kkey );
  if( row >= 0 ) {
    beginRemoveRows( QModelIndex(), row, row );
    mFilterMap.removeAt( row );
    mContens.remove( kkey );
    //Данные изменились, сообщить, что файл грязный
    setDirty();
    endRemoveRows();
    }
  else mContens.remove( kkey );
  }




void SvQmlJsonObjectModelFile::removeMappedRecord(int row)
  {
  //Проверить допуск индексов
  if( row >= 0 && row < mFilterMap.count() )
    removeRecord( mFilterMap.at(row) );
  }




//Очистить базу данных
void SvQmlJsonObjectModelFile::clear()
  {
  beginResetModel();
  mContens = QJsonObject();
  mFilterMap.clear();
  //Данные изменились, сообщить, что файл грязный
  setDirty();
  endResetModel();
  }




void SvQmlJsonObjectModelFile::refilter()
  {
  beginResetModel();
  mFilterMap.clear();
  if( mFilter.isEmpty() )
    mFilterMap = mContens.keys();
  else {
    QStringList list = mContens.keys();
    for( QString str : list )
      if( str.contains( mFilter, Qt::CaseInsensitive ) )
        mFilterMap.append( str );
    }
  endResetModel();
  }



