/*
Project "Интерфейс специальных инструментов с QML"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Директория
*/
#ifndef SVDIR_H
#define SVDIR_H

#include <QString>

class SvDir
  {
    QString mCleanedPath;
  public:
    SvDir( const QString path = QString() );

    QString cleanedPath() const { return mCleanedPath; }

    QString slashedPath() const { return mCleanedPath + QString("/"); }

    void    set( QString path );

    void    up();

    void    cd( const QString name );
  };

#endif // SVDIR_H
