#include "QmlMidiTrack.h"

#include <QDebug>


enum QmlMidiEventField {
  mefTime = Qt::UserRole + 1,
  mefType,
  mefNote,
  mefLenght,
  mefText,
  mefX,
  wefWidth
};



QmlMidiTrack::QmlMidiTrack()
  {

  }



void QmlMidiTrack::beginReadTrack()
  {
  setTrackName( QString{} );
  setInstrumentName( QString{} );
  mRemark.clear();

  beginResetModel();
  mMidiList.clear();
  mTextList.clear();

  //With this first we change current volume level
  mVolume = -1;
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



void QmlMidiTrack::seek(int time)
  {
  //Find event index greater or equal time
  for( mEventIndex = 0; mEventIndex < mMidiList.count(); mEventIndex++ )
    if( mMidiList.at(mEventIndex).mTime >= time ) break;
  }




void QmlMidiTrack::stop()
  {
  //We stops all active notes
  for( int i = mActiveNoteList.count() - 1; i >= 0; i-- )
    //Stop note
    emit midiEvent( 0x10 | mChannel, mMidiList.at( mActiveNoteList.at(i) ).mData0, 0 );
  //... and clear active note list
  mActiveNoteList.clear();
  //With this first we change current volume level
  mVolume = -1;
  }



void QmlMidiTrack::tick(int prevTime, int nextTime, bool soundOn, int volume)
  {
  //Check volume change
  if( volume != mVolume ) {
    //Volume changed, send new volume
    mVolume = volume & 0x7f;
    emit midiEvent( 0xb0 | mChannel, 0x7, mVolume );
    }

  //Test open notes
  for( int i = mActiveNoteList.count() - 1; i >= 0; i-- ) {
    int eventIndex = mActiveNoteList.at(i);
    if( mMidiList.at( eventIndex ).isStopInside(nextTime) ) {
      //This note is ended
      emit midiEvent( 0x10 | mChannel, mMidiList.at( eventIndex ).mData0, 0 );
      //Remove note from active list
      mActiveNoteList.removeAt(i);
      }
    }

  //Test next events
  while( mEventIndex < mMidiList.count() && mMidiList.at(mEventIndex).isStartInside( prevTime, nextTime ) ) {
    //This event may be executed
    if( soundOn ) {
      if( mMidiList.at(mEventIndex).isNote() ) {
        //This is note. Start it
        emit midiEvent( 0x10 | mChannel, mMidiList.at( mEventIndex ).mData0, mMidiList.at( mEventIndex ).mData1 );
        //... and append note to the active note list
        mActiveNoteList.append(mEventIndex);
        }
      else {
        //Simple send command
        if( mMidiList.at(mEventIndex).isMidi() )
          emit midiEvent( mMidiList.at( mEventIndex ).mType | mChannel, mMidiList.at( mEventIndex ).mData0, mMidiList.at( mEventIndex ).mData1 );
        }
      }
    mEventIndex++;
    }

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
  roles.insert( mefX,      "mefX" );
  roles.insert( wefWidth,  "mefX" );
  return roles;
  }
