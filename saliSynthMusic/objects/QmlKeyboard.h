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

#include "SvQml/SvQmlJsonFile.h"

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
    QMap<quint8,int> mKeyMap;        //! Key code association to index in the mKeyList
    int              mDelimiter;     //! Key (key code) which is left of right part of keyboard
    int              mWhiteKeyWidth;
    int              mWhiteKeyCount;
    int              mKeyboardWidth;
    int              mLeftMode;
    int              mRightMode;
    int              mLeftVolume;
    int              mRightMainVolume;
    int              mRightOverlayVolume;
    int              mLeftOffset;
    int              mRightOffset;
    SvQmlJsonFile   *mSettings;

    Q_PROPERTY(int delimiter READ delimiter WRITE setDelimiter NOTIFY delimiterChanged)
    Q_PROPERTY(int whiteKeyWidth READ whiteKeyWidth NOTIFY whiteKeyWidthChanged)
    Q_PROPERTY(int keyboardWidth READ keyboardWidth WRITE setKeyboardWidth NOTIFY keyboardWidthChanged)
    Q_PROPERTY(int leftMode READ leftMode WRITE setLeftMode NOTIFY leftModeChanged)
    Q_PROPERTY(int rightMode READ rightMode WRITE setRightMode NOTIFY rightModeChanged)
    Q_PROPERTY(int leftVolume READ leftVolume WRITE setLeftVolume NOTIFY leftVolumeChanged)
    Q_PROPERTY(int rightMainVolume READ rightMainVolume WRITE setRightMainVolume NOTIFY rightMainVolumeChanged)
    Q_PROPERTY(int rightOverlayVolume READ rightOverlayVolume WRITE setRightOverlayVolume NOTIFY rightOverlayVolumeChanged)
    Q_PROPERTY(SvQmlJsonFile* settings READ settings WRITE setSettings NOTIFY settingsChanged)

  public:
    enum QmlKeyboardMode {
      //Chord detection
      KmChordPassive = 0,
      KmChordActive = 0x1,
      KmChordMask = 0x1,

      KmLeftMask = 0x6,
      KmLeftVoice = 0,
      KmLeftAccomp = 0x2,
      KmLeftArpegio = 0x4,

      KmRightMask = 0x2,
      KmRightSingle = 0,
      KmRightOverlay = 0x2,

      KmRightArpegioMask = 0x30,
      KmRightArpegioMain = 0x10,
      KmRightArpegioOverlay = 0x20
      };
    Q_ENUM(QmlKeyboardMode)


    QmlKeyboard( QObject *parent = nullptr );

    qint8            leftCode() const { return mKeyList.at(0).mCode; }

    int              delimiterCode() const { return mDelimiter; }
    int              delimiter() const { return keyIndex(mDelimiter); }
    void             setDelimiter( int delim );

    int              whiteKeyWidth() const { return mWhiteKeyWidth; }

    int              keyboardWidth() const { return mWhiteKeyWidth * mWhiteKeyCount; }
    void             setKeyboardWidth( int w );

    //Left part of keyboard mode
    int              leftMode() const { return mLeftMode; }
    void             setLeftMode( int mode );

    //Right part of keyboard mode
    int              rightMode() const { return mRightMode; }
    void             setRightMode( int mode );

    //Left part of keyboard voice volume
    int              leftVolume() const { return mLeftVolume; }
    void             setLeftVolume( int vol );

    int              rightMainVolume() const { return mRightMainVolume; }
    void             setRightMainVolume( int vol );

    int              rightOverlayVolume() const { return mRightOverlayVolume; }
    void             setRightOverlayVolume( int vol );


    SvQmlJsonFile   *settings() const { return mSettings; }
    void             setSettings( SvQmlJsonFile *settings );

    Q_INVOKABLE bool keyIsBlack( int keyCode ) const { return mKeyMap.contains(keyCode) ? mKeyList.at( mKeyMap.value(keyCode) ).mIsBlack : false; }
    Q_INVOKABLE int  keyIndex( int keyCode ) const { return mKeyMap.contains(keyCode) ? mKeyList.at( mKeyMap.value(keyCode) ).mIndex : 0; }

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
    void settingsChanged();
    void leftVolumeChanged();
    void rightMainVolumeChanged();
    void rightOverlayVolumeChanged();


    void keyEvent( quint8 cmd, quint8 data0, quint8 data1 );
  public slots:
    void indicate( quint8 keyCode, bool set, quint8 colorMask );

    void build(int count );
  };

#endif // QMLKEYBOARD_H
