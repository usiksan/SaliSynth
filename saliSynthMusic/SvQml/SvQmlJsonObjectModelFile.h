/*
Project "Интерфейс специальных инструментов с QML"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Данные из JsonObject воспринимает как модель прямоугольных данных
*/
#ifndef SVQMLJSONOBJECTMODELFILE_H
#define SVQMLJSONOBJECTMODELFILE_H

#include <QAbstractListModel>
#include <QStringList>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>


class SvQmlJsonObjectModelFile : public QAbstractListModel
  {
    Q_OBJECT

  protected:
    QStringList       mFields;     //Список имен полей таблицы
    QTimer            mAutoSave;   //Автоматическое сохранение через определенный период
    QString           mFileName;   //Имя файла для настроек
    QString           mPath;       //Путь к файлу настроек
    bool              mDirty;      //Обозначает, что объект изменился
    QJsonObject       mContens;    //Содержимое файла JSON
    QStringList       mFilterMap;  //Карта преобразования объектов в номера рядов
    QString           mFilter;     //Текущий фильтр
    bool              mLowerKey;   //Флаг автоматического перевода ключа в нижний регистр

    Q_PROPERTY(QStringList fields READ fields WRITE setFields NOTIFY fieldsChanged)
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(int autoSave READ autoSave WRITE setAutoSave NOTIFY autoSaveChanged)
    Q_PROPERTY(bool isDirty READ isDirty NOTIFY isDirtyChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(bool lowerKey READ lowerKey WRITE setLowerKey NOTIFY lowerKeyChanged)
  public:
    SvQmlJsonObjectModelFile( QObject *parent = nullptr );

    //Доступ к списку полей
    QStringList  fields() const { return mFields; }
    void         setFields( QStringList f );

    //Доступ к имени файла
    QString      file() const { return mFileName; }
    void         setFile( const QString fl );

    //Доступ к пути для файла
    QString      path() const { return mPath; }
    void         setPath( const QString pt );

    //Доступ к свойству автоматического сохранения
    int          autoSave() const;
    void         setAutoSave( int period );

    //Доступ к свойству грязности
    bool         isDirty() const { return mDirty; }

    //Доступ к фильтру
    QString      filter() const { return mFilter; }
    void         setFilter( const QString f );

    //Доступ к флагу перевода ключей в нижний регистр
    bool         lowerKey() const { return mLowerKey; }
    void         setLowerKey( bool low );
  signals:
    //Вызывается при изменении полей
    void         fieldsChanged();

    void         saveComplete( bool success );
    void         loadComplete( bool success );
    void         fileChanged();
    void         pathChanged();
    void         autoSaveChanged();
    void         scanPeriodChanged();
    void         isDirtyChanged();
    void         filterChanged();
    void         lowerKeyChanged();

    // QAbstractItemModel interface
  public:
    virtual int           rowCount(const QModelIndex &parent) const override;
    virtual QVariant      data(const QModelIndex &index, int role) const override;
    virtual bool          setData(const QModelIndex &index, const QVariant &value, int role) override;
//    virtual bool          insertRows(int row, int count, const QModelIndex &parent) override;
//    virtual bool          removeRows(int row, int count, const QModelIndex &parent) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    // SvQmlJsonValue interface
  public slots:
    //Сохранить JSON файл
    bool         save();

    //Сохранить JSON файл под другим именем
    virtual bool saveTo( const QString filePath );

    //Загрузить JSON файл
    bool         load();

    //Загрузить JSON файл под другим именем
    virtual bool loadFrom( const QString filePath );

    //Проверить наличие объекта в головном объекте
    bool         contains( const QString key );
    QString      mappedKey( int row ) const;

    //Получить строку из головного объекта
    QString      asString( const QString key, const QString nm ) const;
    QString      asMappedString( int row, const QString nm ) const;

    //Установить строку в головной объект
    void         setString( const QString key, const QString nm, const QString val );
    void         setMappedString( int row, const QString nm, const QString val );

    //Целое из головного объекта
    int          asInt( const QString key, const QString nm ) const { return asString(key,nm).toInt(); }
    void         setInt( const QString key, const QString nm, int val ) { setString( key, nm, QString::number(val) ); }

    //С плавающей точкой из головного объекта
    double       asReal( const QString key, const QString nm) const;
    int          asFloat1000Int( const QString key, const QString name ) const { return static_cast<int>(asReal( key, name ) * 1000.0); }
    void         setIntAsReal( const QString key, const QString nm, int val, int factor );

    //Установить флаг грязности
    void         setDirty();

    //Сбросить флаг грязности
    void         clearDirty();

    //Добавить запись к таблице
    bool         addRecord( const QString key );

    //Удалить запись из модели
    void         removeRecord( const QString key );
    void         removeMappedRecord( int row );

    //Доступ к данным модели
//    int          asInt( int row, const QString nm );
//    void         setInt( int row, const QString nm, int val );
//    double       asReal(int row, const QString nm);
//    void         setIntAsReal( int row, const QString nm, int val, int factor );

    //Количество записей в базе
    int          count() const { return mContens.count(); }
    int          mappedCount() const { return mFilterMap.count(); }

    //Очистить базу данных
    void         clear();

  private:
    //Перестроить фильтр
    void         refilter();
  };

#endif // SVQMLJSONOBJECTMODELFILE_H
