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
#include "SvTime2x.h"
#include <QDebug>



//Преобразовать в эпоху
qint32 SvTime2x::current()
  {
  return fromEpoch( QDateTime::currentSecsSinceEpoch() );
  }



//Преобразовать в эпоху
qint64 SvTime2x::toEpoch(qint32 tm)
  {
  return SV_TIME_2X_START + static_cast<qint64>(tm);
  }




//Преобразовать из эпохи
qint32 SvTime2x::fromEpoch(qint64 tm)
  {
  //При отсутствии времени возвращаем ноль обозначающий тоже отсутствие времени
  if( tm == 0 )
    return 0;
  return static_cast<qint32>( tm - SV_TIME_2X_START);
  }




//Получить реальную дату
QDateTime SvTime2x::toUtc(qint32 tm)
  {
  if( tm <= 0 )
    return QDateTime();
  return QDateTime::fromSecsSinceEpoch( toEpoch(tm), Qt::UTC );
  }




//Получить реальную локальную дату
QDateTime SvTime2x::toLocal(qint32 tm)
  {
  if( tm <= 0 )
    return QDateTime();
  return QDateTime::fromSecsSinceEpoch( toEpoch(tm) );
  }




//Преобразовать из момента
qint32 SvTime2x::fromDateTime(const QDateTime tm)
  {
  return fromEpoch( tm.toSecsSinceEpoch() );
  }



