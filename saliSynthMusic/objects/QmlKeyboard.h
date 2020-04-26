/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   QmlKeyboard class Keyboard for visual representation
   Keyboard allow to press key on the screen and display pressing
   from midi stream.
   Keyboard consists from array of keys. Each of them may be white
   or black. This define by mIsBlack field of QmlKey struct.
   Each key has keyboard code (midi code).

   Keyboard separate on two parts: left and right.
   Delimiter between thees parts can be ajust.
   Left part mode:
     - single instrument
     - autoaccompaniator
   Play left part may be
     - simple play
     - yamaha chord mode
*/
#ifndef QMLKEYBOARD_H
#define QMLKEYBOARD_H

#include <QAbstractListModel>
#include <QList>
#include <QMap>

//!
//! \brief The QmlKey struct Keyboard key for visual representation
//!
struct QmlKey {
    quint8 mCode;      //! Key code
    bool   mIsBlack;   //! If true then key is black
    quint8 mColorMask; //! Key color bit set
    int    mIndex;     //! Key index in keyboard
  };


//!
//! \brief The QmlKeyboard class Keyboard for visual representation
//!
class QmlKeyboard : public QAbstractListModel
  {
    Q_OBJECT

    QList<QmlKey>    mKeyList;
    QMap<quint8,int> mKeyMap;
    int              mDelimiter;
    int              mWhiteKeyWidth;
    int              mWhiteKeyCount;
    int              mKeyboardWidth;
    int              mLeftMode;
    int              mRightMode;

    Q_PROPERTY(int delimiter READ delimiter WRITE setDelimiter NOTIFY delimiterChanged)
    Q_PROPERTY(int whiteKeyWidth READ whiteKeyWidth NOTIFY whiteKeyWidthChanged)
    Q_PROPERTY(int keyboardWidth READ keyboardWidth WRITE setKeyboardWidth NOTIFY keyboardWidthChanged)
    Q_PROPERTY(int leftMode READ leftMode WRITE setLeftMode NOTIFY leftModeChanged)
    Q_PROPERTY(int rightMode READ rightMode WRITE setRightMode NOTIFY rightModeChanged)

  public:
    QmlKeyboard( QObject *parent = nullptr );

    qint8 leftCode() const { return mKeyList.at(0).mCode; }

    int  delimiterCode() const { return mDelimiter; }
    int  delimiter() const { return mKeyMap.contains(mDelimiter) ? mKeyList.at(mKeyMap.value(mDelimiter)).mIndex : 0; }
    void setDelimiter( int delim );

    int  whiteKeyWidth() const { return mWhiteKeyWidth; }

    int  keyboardWidth() const { return mWhiteKeyWidth * mWhiteKeyCount; }
    void setKeyboardWidth( int w );

    int  leftMode() const { return mLeftMode; }
    void setLeftMode( int mode );

    int  rightMode() const { return mRightMode; }
    void setRightMode( int mode );

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
    void leftModeChanged();
    void rightModeChanged();

    void keyEvent( quint8 cmd, quint8 data0, quint8 data1 );
  public slots:
    void indicate( quint8 keyCode, bool set, quint8 colorMask );

    void build(int count );
  };

#endif // QMLKEYBOARD_H
