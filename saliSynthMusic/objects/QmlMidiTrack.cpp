#include "QmlMidiTrack.h"

#include <QDebug>


enum QmlMidiEventField {
  mefTime = Qt::UserRole + 1,
  mefType,
  mefNote,
  mefLenght,
  mefText
};



QmlMidiTrack::QmlMidiTrack()
  {

  }

void QmlMidiTrack::beginReadTrack()
  {
  setTrackName( QString{} );
  setInstrumentName( QString{} );

  beginResetModel();
  mMidiList.clear();
  mTextList.clear();

  }

void QmlMidiTrack::endReadTrack()
  {
  endResetModel();
  }




void QmlMidiTrack::addMidiEvent(quint32 time, quint8 statusByte, quint8 data0, quint8 data1)
  {
  quint8 cmd = statusByte & 0x70;
  if( cmd == 0 || (cmd == 0x10 && data1 == 0) ) {
    //Note off
    if( mActiveNotesMap.contains(data0) ) {
      int eventIndex = mActiveNotesMap.value(data0);
      mMidiList[eventIndex].mLenght = time - mMidiList[eventIndex].mTime;
      mActiveNotesMap.remove(data0);
      }
    else qDebug() << "hanging note off" << data0;
    }
  else if( cmd == 0x10 ) {
    //Note on
    QmlMidiEvent ev;
    ev.mTime = time;
    ev.mType = cmd;
    ev.mData0 = data0;
    ev.mData1 = data1;
    ev.mLenght = 0;
    if( mActiveNotesMap.contains(data0) ) {
      //Note on when it already on
      //Close previous note
      addMidiEvent( time, statusByte, data0, 0 );
      }
    //Append active note
    mActiveNotesMap.insert( data0, mMidiList.count() );
    //Append note to the list
    mMidiList.append( ev );
    }
  else {
    if( cmd == 0x30 && data0 == 0 )
      //Bank MSB
      mVoiceId.mBankMsb = data1 & 0x7f;
    else if( cmd == 0x30 && data0 == 0x20 )
      //Bank LSB
      mVoiceId.mBankLsb = data1 & 0x7f;
    else if( cmd == 0x40 )
      //Program
      mVoiceId.mProgramm = data0;
    else {
      //All others commands
      QmlMidiEvent ev;
      ev.mTime = time;
      ev.mType = cmd;
      ev.mData0 = data0;
      ev.mData1 = data1;
      ev.mLenght = 0;
      mMidiList.append( ev );
      }
    }
  }



void QmlMidiTrack::addMidiText(quint32 time, qint16 type, const QString text)
  {
  QmlMidiEvent ev;
  ev.mTime = time;
  ev.mType = type;
  ev.mTextIndex = mTextList.count();
  ev.mData0 = ev.mData1 = 0;
  mTextList.append( text );
  mMidiList.append( ev );
  }




void QmlMidiTrack::setTrackName(const QString nm)
  {
  mTrackName = nm;
  emit trackNameChanged();
  }

void QmlMidiTrack::setInstrumentName(const QString nm)
  {
  mInstrumentName = nm;
  emit instrumentNameChanged();
  }


int QmlMidiTrack::rowCount(const QModelIndex &parent) const
  {
  Q_UNUSED(parent)
  return mMidiList.count();
  }



QVariant QmlMidiTrack::data(const QModelIndex &index, int role) const
  {
  //Check index bound
  if( !index.isValid() || index.row() >= mMidiList.count() || index.row() < 0 || role <= Qt::UserRole )
    return QVariant();

  switch( role ) {
    case mefTime    : return QVariant( mMidiList.at(index.row()).mTime );
    case mefType    : return QVariant( mMidiList.at(index.row()).mType );
    case mefNote    : return QVariant( mMidiList.at(index.row()).mData0 );
    case mefLenght  : return QVariant( mMidiList.at(index.row()).mLenght );
    case mefText    : return mMidiList.at(index.row()).mType == 0x100 ? QVariant( mTextList.at( mMidiList.at(index.row()).mTextIndex ) ) : QVariant(QString{});
    }

  return QVariant();
  }



bool QmlMidiTrack::setData(const QModelIndex &index, const QVariant &value, int role)
  {
  }

bool QmlMidiTrack::insertRows(int row, int count, const QModelIndex &parent)
  {
  }



Qt::ItemFlags QmlMidiTrack::flags(const QModelIndex &index) const
  {
  Q_UNUSED( index )
  return Qt::ItemIsEditable | Qt::ItemIsEnabled;
  }



QHash<int, QByteArray> QmlMidiTrack::roleNames() const
  {
  QHash<int, QByteArray> roles;
  roles.insert( mefTime,   "mefTime" );
  roles.insert( mefType,   "mefType" );
  roles.insert( mefNote,   "mefNote" );
  roles.insert( mefLenght, "mefLenght" );
  roles.insert( mefText,   "mefText" );
  return roles;
  }
