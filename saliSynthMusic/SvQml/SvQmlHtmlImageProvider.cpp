/*
Project "Интерфейс специальных инструментов с QML"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  HtmlImageProvider class предоставляет картинку для Image в QML, в которой отрисовано содержиимое HTML-документа
*/
#include "SvQmlHtmlImageProvider.h"

#include <QFile>
#include <QPainter>
#include <QDebug>
#include <QAbstractTextDocumentLayout>

SvQmlHtmlImageProvider::SvQmlHtmlImageProvider(QObject *parent) :
  QObject(parent),
  QQuickImageProvider(QQuickImageProvider::Image),
  mPageWidth(0)
  {

  }




//Доступ к картинке с заданным идентификатором
QImage SvQmlHtmlImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
  {
  Q_UNUSED(id)
  Q_UNUSED(requestedSize)

  //Размер документа
  QSize docSize = mCurrentDocument.size().toSize();

  //Создать картинку, соответствующую размеру документа
  QImage img(docSize, QImage::Format_RGBA8888_Premultiplied);
  img.fill(Qt::transparent);

  //Создать рисовальщик
  QPainter painter(&img);

  //Если задано вернуть результат, то вернуть
  if( size != nullptr )
    *size = docSize;

  //Рисовать содержимое
  mCurrentDocument.drawContents(&painter);

  //Вернуть картинку
  return img;
  }





//Путь к текущему документу
void SvQmlHtmlImageProvider::setCurrentFName(const QString &fname)
  {
  if( mCurrentFName == fname )
    return;

  mCurrentFName = fname;

  QString path = mBasePath + mCurrentFName;
  QFile file(path);
  if( !file.open(QFile::ReadOnly) ) {
    qWarning() << "Can't open file" << path;
    return;
    }

  mCurrentDocument.setHtml( QString::fromUtf8(file.readAll()) );

  emit currentFNameChanged();
  emit imageUpdated();
  }






//Базовый путь
void SvQmlHtmlImageProvider::setBasePath(const QString &basePath)
  {
  if( mBasePath == basePath )
    return;

  mBasePath = basePath;
  if( !mBasePath.endsWith('/') )
    mBasePath.append('/');
  QUrl url = QUrl::fromLocalFile(mBasePath);
  mCurrentDocument.setBaseUrl(url);

  emit basePathChanged();
  emit imageUpdated();
  }






//Доступ к ширине страницы
void SvQmlHtmlImageProvider::setPageWidth(int pageWidth)
  {
  if( mPageWidth == pageWidth )
    return;

  mPageWidth = pageWidth;
  mCurrentDocument.setTextWidth(pageWidth);

  emit pageWidthChanged();
  emit imageUpdated();
  }





//Получение ссылки по точке нажатия
QString SvQmlHtmlImageProvider::linkAt(const QPoint &point)
  {
  QAbstractTextDocumentLayout *docLayout = mCurrentDocument.documentLayout();
  if( docLayout != nullptr ) {
    //Получить ссылку в области нажатия
    return docLayout->anchorAt( point );
    }
  return QString();
  }




//Добавить текущий путь в стек
void SvQmlHtmlImageProvider::historyPush()
  {
  //Если последний элемент не совпадает с добавляемым, то добавляем в список истории
  if( mHistory.isEmpty() || mHistory.last() != mCurrentFName ) {
    mHistory.append( mCurrentFName );
    emit historyEmptyChanged();
    }
  }




//Достать текущий путь из стека
void SvQmlHtmlImageProvider::historyPop()
  {
  //Если есть элементы, то достать из списка
  if( !mHistory.isEmpty() ) {
    setCurrentFName( mHistory.last() );
    mHistory.removeLast();
    emit historyEmptyChanged();
    }
  }





//Очистить историю перемещений
void SvQmlHtmlImageProvider::historyClear()
  {
  mHistory.clear();
  emit historyEmptyChanged();
  }











