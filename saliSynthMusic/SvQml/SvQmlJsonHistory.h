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
#ifndef SVQMLJSONHISTORY_H
#define SVQMLJSONHISTORY_H

#include "SvQmlJsonValue.h"
#include <QStringList>

class SvQmlJsonHistory : public SvQmlJsonValue
  {
    Q_OBJECT

    QStringList mList;  //Список строк истории
    int         mLimit; //Ограничение количества строк. При его превышении последняя строка из списка удаляется.

    Q_PROPERTY(QStringList list READ list NOTIFY listChanged)
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
  public:
    SvQmlJsonHistory(QObject *parent = nullptr);

    //Доступ ко всему списку строк
    QStringList list() const { return mList; }

    //Доступ к ограничителю количества строк
    int         limit() const { return mLimit; }
    void        setLimit( int lim );

  signals:
    void limitChanged();
    void listChanged();

  public slots:
    //Очистить список
    void clear();

    //Добавить строку в начало списка
    void push( const QString str );

    //Добавить строку в конец списка
    void append( const QString str );

    //Добавить уникальную строку. Заданная строка становится первой в списке.
    void insertUnical( const QString str );

    //Вызывается из JSON после загрузки объекта, чтобы скорректировать значение в соответствии с загруженным
    virtual void onLoadJson();

    //Вызывается из JSON перед сохранением объекта, чтобы обновить в модели файла все значения
    virtual void onSaveJson();


  private:
    //Обновить список в json
    void updateToJson();
  };

#endif // SVQMLJSONHISTORY_H
