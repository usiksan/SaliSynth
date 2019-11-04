/*
Project "Универсальная система хранения архивов"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Единые отсчеты времени.

  Все время отсчитывается в 32-разрядных секундах от начала 2018 года.
*/
#ifndef SVTIME2X_H
#define SVTIME2X_H

#include <QDateTime>

//Момент начала отсчета временных интервалов от начала 2018 года
#define SV_TIME_2X_START 1514764800l

class SvTime2x
  {
  public:
    //Текущий глобальный момент
    static qint32    current();

    //Взаимодействие со временем относительно эпохи
    //Преобразовать в эпоху
    static qint64    toEpoch( qint32 tm );

    //Преобразовать из эпохи
    static qint32    fromEpoch( qint64 tm );


    //Получить реальную дату
    static QDateTime toUtc( qint32 tm );

    //Получить реальную локальную дату
    static QDateTime toLocal( qint32 tm );

    //Преобразовать из момента
    static qint32    fromDateTime( const QDateTime tm );
  };

#endif // SVTIME2X_H
