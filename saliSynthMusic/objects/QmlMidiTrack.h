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

    bool isNote() const { return mType == 0x10; }

    bool isMidi() const { return (mType & 0xff00) == 0; }

    bool isStartInside( int firstTime, int lastTime ) const { return firstTime <= mTime && mTime < lastTime; }

    bool isStopInside( int lastTime ) const { return (mTime + mLenght) < lastTime; }
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
    QMap<quint8,int> mActiveNotesMap; //! Active note map used in read process to collect note on and note off midi commands
    QList<int>       mActiveNoteList; //! Active note list to detect on-off notes
    qint32           mEventIndex;     //! Next event to play
    quint8           mChannel;        //! Channel on which track is plaied
    int              mVolume;         //! Current track volume

    Q_PROPERTY(QString trackName READ trackName WRITE setTrackName NOTIFY trackNameChanged)
    Q_PROPERTY(QString instrumentName READ instrumentName WRITE setInstrumentName NOTIFY instrumentNameChanged)
  public:
    QString          mRemark;

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

    void    seek( int time );

    void    stop();

    void    tick( int prevTime, int nextTime, bool soundOn, int volume );

  signals:
    void trackNameChanged();
    void instrumentNameChanged();

    void midiEvent( quint8 cmd, quint8 data0, quint8 data1 );

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
