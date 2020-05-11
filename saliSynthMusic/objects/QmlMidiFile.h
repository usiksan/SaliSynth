/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   MIDI file
*/
#ifndef QMLMIDIFILE_H
#define QMLMIDIFILE_H

#include "iff/IffReader.h"
#include "QmlMidiTrack.h"
#include "SvQml/SvQmlJsonModel.h"

#include <QList>
#include <QAbstractListModel>
#include <QTimer>

#define TRACK_ID      QStringLiteral("trackId")
#define TRACK_NAME    QStringLiteral("trackName")
#define TRACK_INDEX   QStringLiteral("trackIndex")
#define TRACK_ON      QStringLiteral("trackOn")
#define TRACK_REMARK  QStringLiteral("trackRemark")
#define TRACK_VOLUME  QStringLiteral("trackVolume")
#define TRACK_VISIBLE QStringLiteral("trackVisible")
#define TRACK_COLOR   QStringLiteral("trackColor")


struct QmlMidiMarker {
    QString mMarker;
    qint32  mTime;
  };

using QmlMidiMarkerList = QList<QmlMidiMarker>;

class QmlMidiFile : public QObject
  {
    Q_OBJECT

  protected:
    quint16           mFormat;       //! Midi file format (0 or 1)
    quint16           mTrackNumber;  //! Track count
    quint16           mDivision;
    quint32           mTempo;

    QmlMidiTrack      mQmlTrack[16];  //! Track in midi file with visual representation
    SvQmlJsonModel    mQmlTrackModel; //! Model represents visual info about midi file tracks
    QmlMidiMarkerList mMarkerList;    //! List of marker with time position

    QString           mMidiName;      //! Midi file name without path
    QString           mConfigFile;
    bool              mConfigDirty;

    QTimer            mTimer;         //! Timer for playback
    qint32            mTickCount;     //! Current time tick value
    qint32            mTickStep;      //! Current time tick step for playback
    quint32           mFileLenght;    //! File lenght in time tick
    bool              mPause;         //! Pause when playing

    Q_PROPERTY(int tickCount READ tickCount WRITE setTickCount NOTIFY tickCountChanged)
    Q_PROPERTY(int tickStep READ tickStep WRITE setTickStep NOTIFY tickStepChanged)
    Q_PROPERTY(int fileLenght READ fileLenght WRITE setFileLenght NOTIFY fileLenghtChanged)
    Q_PROPERTY(QString midiName READ midiName NOTIFY midiNameChanged)
  public:
    QmlMidiFile( QObject *parent = nullptr );

    void configRead( QString fname );

    int  tickCount() const { return mTickCount >> 4; }
    void setTickCount( int tc );

    int  tickStep() const { return mTickStep; }
    void setTickStep( int stp );

    int  fileLenght() const { return mFileLenght; }
    void setFileLenght( int len ) { mFileLenght = len; emit fileLenghtChanged(); }

    QString midiName() const { return mMidiName; }

    Q_INVOKABLE QmlMidiTrack *qmlMidiTrack( int track ) { return mQmlTrack + (track & 0xf); }

    Q_INVOKABLE SvQmlJsonModel *qmlMidiTrackModel() { return &mQmlTrackModel; }
  signals:
    void tickCountChanged();

    void tickStepChanged();

    void fileLenghtChanged();

    void midiNameChanged();

    void midiEvent( quint8 cmd, quint8 data0, quint8 data1 );

    void voiceSetup( quint8 channel, int voiceId );

  public slots:
    //!
    //! \brief tick Elementary tick. On this tick player build notes
    //!
    virtual void tick();

    //!
    //! \brief play Begin to play
    //!
    void         play();

    //!
    //! \brief pause Pause playing
    //!
    void         pause();

    //!
    //! \brief stop Stop play
    //!
    virtual void stop();

    bool read( QString fname );

    void configWrite();

  protected:
    bool         readMthd( IffReader &reader );
    void         readMtrk( IffReader &reader );
    virtual void readExtension( IffReader &reader );
    quint32      variableLenValue( IffReader &reader );
    virtual void postRead();

    //!
    //! \brief stopAllNotes Stop all playing notes
    //!
    void         stopAllNotes();

  };

#endif // QMLMIDIFILE_H
