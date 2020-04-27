#include "QmlKeyboard.h"

#define QML_KEY_COLOR    (Qt::UserRole + 1)
#define QML_KEY_PRESENT  (Qt::UserRole + 2)
#define QML_KEY_INDEX    (Qt::UserRole + 3)
#define QML_KEY_IS_BLACK (Qt::UserRole + 4)


QmlKeyboard::QmlKeyboard(QObject *parent) :
  QAbstractListModel(parent),
  mDelimiter(48),
  mLeftMode(1),
  mRightMode(0)
  {

  }




void QmlKeyboard::setDelimiter(int delim)
  {
  mDelimiter = delim;
  emit delimiterChanged();
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
  }




void QmlKeyboard::setRightMode(int mode)
  {
  mRightMode = mode;
  emit rightModeChanged();
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

    if( mDelimiter < 0 ) {
      mDelimiter = keyCode;
      emit delimiterChanged();
      }
    }
  }




void QmlKeyboard::build(int count)
  {
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


