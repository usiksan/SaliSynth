#ifndef QMLMIDITRACK_H
#define QMLMIDITRACK_H

#include "soundFont/SfVoiceId.h"

#include <QAbstractListModel>
#include <QList>
#include <QMap>

#define MEV_CMD    0
#define MEV_MARKER 0x100
#define MEV_TEXT   0x200
#define MEV_LYRIC  0x200


struct QmlMidiEvent {
    qint32 mTime;
    union {
        qint16 mLenght;
        qint16 mTextIndex;
      };
    qint16 mType;
    quint8 mData0;
    quint8 mData1;
  };


using QmlMidiEventList = QList<QmlMidiEvent>;



class QmlMidiTrack : public QAbstractListModel
  {
    Q_OBJECT

    QString          mTrackName;
    QString          mInstrumentName;
    SfVoiceId        mVoiceId;

    QmlMidiEventList mMidiList;
    QStringList      mTextList;
    QMap<quint8,int> mActiveNotesMap;
    quint8           mChannel;

  public:
    int              mModelRow;

    Q_PROPERTY(QString trackName READ trackName WRITE setTrackName NOTIFY trackNameChanged)
    Q_PROPERTY(QString instrumentName READ instrumentName WRITE setInstrumentName NOTIFY instrumentNameChanged)
  public:

    QmlMidiTrack();

    void beginReadTrack();
    void endReadTrack();
    void addMidiEvent( quint32 time, quint8 statusByte, quint8 data0, quint8 data1 = 0 );
    void addMidiText( quint32 time, qint16 type, const QString text );

    //Track name access
    QString trackName() const { return mTrackName; }
    void    setTrackName( const QString nm );

    //Instrument name access
    QString instrumentName() const { return mInstrumentName; }
    void    setInstrumentName( const QString nm );

    //Voice id access
    int     voiceId() const { return mVoiceId.mVoiceId; }
    void    setVoiceId( int id ) { mVoiceId.mVoiceId = id; }

    //Channel
    quint8  channel() const { return mChannel; }
    void    setChannel( quint8 ch ) { mChannel = ch; }

    bool    isActive() const { return mMidiList.count() != 0; }
  signals:
    void trackNameChanged();
    void instrumentNameChanged();

    // QAbstractItemModel interface
  public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
  };

#endif // QMLMIDITRACK_H
