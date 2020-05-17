#include "QmlKeyboard.h"

#define QML_KEY_COLOR    (Qt::UserRole + 1)
#define QML_KEY_PRESENT  (Qt::UserRole + 2)
#define QML_KEY_INDEX    (Qt::UserRole + 3)
#define QML_KEY_IS_BLACK (Qt::UserRole + 4)


QmlKeyboard::QmlKeyboard(QObject *parent) :
  QAbstractListModel(parent),
  mDelimiter(48),
  mLeftMode(1),
  mRightMode(0),
  mLeftVolume(127),
  mRightMainVolume(127),
  mRightOverlayVolume(127),
  mSettings(nullptr)
  {

  }




void QmlKeyboard::setDelimiter(int delim)
  {
  mDelimiter = delim;
  emit delimiterChanged();
  if( mSettings )
    mSettings->setInt( QStringLiteral("delimiter"), mDelimiter );
  }





void QmlKeyboard::setKeyboardWidth(int w)
  {
  mWhiteKeyWidth = w / mWhiteKeyCount;
  emit whiteKeyWidthChanged();
  emit keyboardWidthChanged();
  }




void QmlKeyboard::setLeftMode(int mode)
  {
  mLeftMode = mode;
  emit leftModeChanged();
  if( mSettings )
    mSettings->setInt( QStringLiteral("left mode"), mLeftMode );
  }




void QmlKeyboard::setRightMode(int mode)
  {
  mRightMode = mode;
  emit rightModeChanged();
  if( mSettings )
    mSettings->setInt( QStringLiteral("right mode"), mRightMode );
  }




void QmlKeyboard::setLeftVolume(int vol)
  {
  mLeftVolume = qBound( 0, vol, 127 );
  emit leftVolumeChanged();
  if( mSettings )
    mSettings->setInt( QStringLiteral("left volume"), mLeftVolume );
  emit keyEvent( 0xb0 | 0x1, 0x7, mLeftVolume );
  }




void QmlKeyboard::setRightMainVolume(int vol)
  {
  mRightMainVolume = qBound( 0, vol, 127 );
  emit leftVolumeChanged();
  if( mSettings )
    mSettings->setInt( QStringLiteral("right main volume"), mRightMainVolume );
  emit keyEvent( 0xb0 | 0x0, 0x7, mRightMainVolume );
  }





void QmlKeyboard::setRightOverlayVolume(int vol)
  {
  mRightOverlayVolume = qBound( 0, vol, 127 );
  emit leftVolumeChanged();
  if( mSettings )
    mSettings->setInt( QStringLiteral("right overlay volume"), mRightOverlayVolume );
  emit keyEvent( 0xb0 | 0x2, 0x7, mRightOverlayVolume );
  }




void QmlKeyboard::setSettings(SvQmlJsonFile *settings)
  {
  //Read settings
  build( settings->asIntDefault( QStringLiteral("keyboard keys"), 61 ) );

  setDelimiter( settings->asIntDefault( QStringLiteral("delimiter"), 48 ) );

  setLeftMode( settings->asIntDefault( QStringLiteral("left mode"), 1 ) );

  setRightMode( settings->asIntDefault( QStringLiteral("right mode"), 0 ) );

  setLeftVolume( settings->asIntDefault( QStringLiteral("left volume"), 127) );
  setRightMainVolume( settings->asIntDefault( QStringLiteral("right main volume"), 127) );
  setRightOverlayVolume( settings->asIntDefault( QStringLiteral("right overlay volume"), 127) );

  mSettings = settings;
  }



int QmlKeyboard::rowCount(const QModelIndex &parent) const
  {
  Q_UNUSED(parent)
  return mKeyList.count();
  }




QVariant QmlKeyboard::data(const QModelIndex &index, int role) const
  {
  //Check index bound
  if( !index.isValid() || index.row() >= mKeyList.count() || index.row() < 0 || role <= Qt::UserRole )
    return QVariant();

  switch( role ) {
    case QML_KEY_COLOR    : return QVariant( mKeyList.at(index.row()).mColorMask );
    case QML_KEY_INDEX    : return QVariant( mKeyList.at(index.row()).mIndex );
    case QML_KEY_IS_BLACK : return QVariant( mKeyList.at(index.row()).mIsBlack );
    }

  return QVariant();
  }



bool QmlKeyboard::setData(const QModelIndex &index, const QVariant &value, int role)
  {
  //Check index bound
  if( !index.isValid() || index.row() >= mKeyList.count() || index.row() < 0 || role <= Qt::UserRole )
    return false;

  if( role == QML_KEY_COLOR ) {
    //Simulate key press or release
    //Get key code
    int keyCode = mKeyList.at( index.row() ).mCode;
    if( value.toBool() ) {
      //Simulate key press
      mKeyList[index.row()].mColorMask |= 1;
      emit keyEvent( 16, keyCode, 99 );
      }
    else {
      //Simulate key release
      mKeyList[index.row()].mColorMask &= ~1;
      emit keyEvent( 16, keyCode, 0 );
      }
    emit dataChanged( index, index, {QML_KEY_COLOR} );
    }
  return true;
  }


Qt::ItemFlags QmlKeyboard::flags(const QModelIndex &index) const
  {
  Q_UNUSED( index )
  return Qt::ItemIsEditable | Qt::ItemIsEnabled;
  }




QHash<int, QByteArray> QmlKeyboard::roleNames() const
  {
  QHash<int, QByteArray> roles;
  roles.insert( QML_KEY_COLOR,    "keyColor" );
  roles.insert( QML_KEY_INDEX,    "keyIndex" );
  roles.insert( QML_KEY_IS_BLACK, "keyIsBlack" );
  return roles;
  }







void QmlKeyboard::indicate(quint8 keyCode, bool set, quint8 colorMask)
  {
  if( mKeyMap.contains(keyCode) ) {
    int keyIndex = mKeyMap.value(keyCode);
    if( set ) mKeyList[ keyIndex ].mColorMask |= colorMask;
    else      mKeyList[ keyIndex ].mColorMask &= ~colorMask;
    //Notify that model changed
    emit dataChanged( index(keyIndex), index(keyIndex), {QML_KEY_COLOR} );

    if( mDelimiter < 0 )
      setDelimiter( keyCode );
    }
  }




void QmlKeyboard::build(int count)
  {
  if( mSettings )
    mSettings->setInt( QStringLiteral("keyboard keys"), count );

  beginResetModel();
  mKeyList.clear();
  mKeyMap.clear();
  if( count == 61 ) {
    //Build 61-key keyboard
    quint8 code = 36;
    int whiteKeyIndex = 0;
    int blackKeyIndex = 0;
    for( int i = 0; i < count; i++ ) {
      QmlKey key;

      //White key
      key.mCode = code++;
      key.mIndex = whiteKeyIndex++;
      key.mIsBlack = false;
      key.mColorMask = 0;
      mKeyMap.insert( key.mCode, mKeyList.count() );
      mKeyList.append( key );

      //Black key
      if( (blackKeyIndex % 7) == 2 || (blackKeyIndex % 7) == 6 )
        blackKeyIndex++;
      else {
        i++;
        if( i < count ) {
          key.mCode = code++;
          key.mIndex = blackKeyIndex++;
          key.mIsBlack = true;
          key.mColorMask = 0;
          mKeyMap.insert( key.mCode, mKeyList.count() );
          mKeyList.append( key );
          }
        }
      }
    mWhiteKeyCount = whiteKeyIndex;
    //mWhiteKeyWidth = 22;
    }
  endResetModel();
  }


