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
    int              mDelimiter;
    int              mWhiteKeyWidth;
    int              mWhiteKeyCount;
    int              mKeyboardWidth;

    Q_PROPERTY(int delimiter READ delimiter WRITE setDelimiter NOTIFY delimiterChanged)
    Q_PROPERTY(int whiteKeyWidth READ whiteKeyWidth NOTIFY whiteKeyWidthChanged)
    Q_PROPERTY(int keyboardWidth READ keyboardWidth WRITE setKeyboardWidth NOTIFY keyboardWidthChanged)

  public:
    QmlKeyboard( QObject *parent = nullptr );

    qint8 leftCode() const { return mKeyList.at(0).mCode; }

    int  delimiterCode() const { return mDelimiter; }
    int  delimiter() const { return mKeyMap.contains(mDelimiter) ? mKeyList.at(mKeyMap.value(mDelimiter)).mIndex : 0; }
    void setDelimiter( int delim );

    int  whiteKeyWidth() const { return mWhiteKeyWidth; }

    int  keyboardWidth() const { return mWhiteKeyWidth * mWhiteKeyCount; }
    void setKeyboardWidth( int w );

    // QAbstractItemModel interface
  public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

  signals:
    void delimiterChanged();
    void whiteKeyWidthChanged();
    void keyboardWidthChanged();

    void keyEvent( quint8 cmd, quint8 data0, quint8 data1 );
  public slots:
    void indicate( quint8 keyCode, bool set, quint8 colorMask );

    void build(int count );
  };

#endif // QMLKEYBOARD_H
