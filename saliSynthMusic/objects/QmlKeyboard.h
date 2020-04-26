#ifndef QMLKEYBOARD_H
#define QMLKEYBOARD_H

#include <QAbstractListModel>
#include <QList>
#include <QMap>

struct QmlKey {
    quint8 mCode;      //! Key code
    bool   mIsBlack;   //! If true then key is black
    quint8 mColorMask; //! Key color bit set
    int    mIndex;     //! Key index in keyboard
  };

class QmlKeyboard : public QAbstractListModel
  {
    Q_OBJECT

    QList<QmlKey>    mKeyList;
    QMap<quint8,int> mKeyMap;
  public:
    QmlKeyboard( QObject *parent = nullptr );

    // QAbstractItemModel interface
  public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

  signals:
    void keyEvent( quint8 cmd, quint8 data0, quint8 data1 );

  public slots:
    void indicate( quint8 keyCode, bool set, quint8 colorMask );

    void build(int count );
  };

#endif // QMLKEYBOARD_H
