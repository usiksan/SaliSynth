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
#ifndef SVQMLHTMLIMAGEPROVIDER_H
#define SVQMLHTMLIMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QTextDocument>
#include <QStringList>

///
/// \brief The SvQmlHtmlImageProvider class предоставляет картинку для Image в QML, в которой отрисовано содержиимое HTML-документа
/// для получения ссылки в точке картинки используется linkAt
/// имя файла для просмотра currentPath относительно пути basePath для разрешения ссылок
/// для изменения ширины картинки используется setPageWidth
///
class SvQmlHtmlImageProvider : public QObject, public QQuickImageProvider
  {
    Q_OBJECT

    QString       mCurrentFName;    //Имя файла текущего документу
    QTextDocument mCurrentDocument; //Текущий документ
    QString       mBasePath;        //Базовый путь к системе помощи
    int           mPageWidth;       //Ширина страницы помощи

    QStringList   mHistory;         //История перемещений

    Q_PROPERTY(QString currentFName READ currentFName WRITE setCurrentFName NOTIFY currentFNameChanged)
    Q_PROPERTY(QString basePath READ basePath WRITE setBasePath NOTIFY basePathChanged)
    Q_PROPERTY(int pageWidth READ pageWidth WRITE setPageWidth NOTIFY pageWidthChanged )
    Q_PROPERTY(bool historyEmpty READ historyEmpty NOTIFY historyEmptyChanged)
  public:
    explicit SvQmlHtmlImageProvider(QObject *parent = nullptr);

    //Доступ к картинке с заданным идентификатором
    QImage  requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    //Путь к текущему документу
    QString currentFName() const { return mCurrentFName; }
    void    setCurrentFName(const QString &fname);

    //Базовый путь
    QString basePath() const { return mBasePath; }
    void    setBasePath(const QString &basePath);

    //Доступ к ширине страницы
    int     pageWidth() const { return mPageWidth; }
    void    setPageWidth(int pageWidth);

    //Индикатор пустоты стека посещений
    bool    historyEmpty() const { return mHistory.isEmpty(); }
  signals:
    void currentFNameChanged();
    void basePathChanged();
    void imageUpdated();
    void pageWidthChanged();
    void historyEmptyChanged();

  public slots:
    //Получение ссылки по точке нажатия
    QString linkAt(const QPoint & point);

    //Добавить текущий путь в стек
    void    historyPush();

    //Достать текущий путь из стека
    void    historyPop();

    //Очистить историю перемещений
    void    historyClear();
  };

#endif // SVQMLHTMLIMAGEPROVIDER_H
