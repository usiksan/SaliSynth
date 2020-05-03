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


class QmlMidiFile : public QObject
  {
    Q_OBJECT

  protected:
    quint16         mFormat;      //Midi file format (0 or 1)
    quint16         mTrackNumber;
    quint16         mDivision;
    quint32         mTempo;

    QmlMidiTrack    mQmlTrack[16];  //Track in midi file with visual representation
    SvQmlJsonModel  mQmlTrackModel; //! Model represents visual info about midi file tracks

    QString         mConfigFile;
    bool            mConfigDirty;

    QTimer          mTimer;
    qint32          mTickCount;
    qint32          mTickStep;
    quint32         mFileLenght;

    Q_PROPERTY(int tickCount READ tickCount WRITE setTickCount NOTIFY tickCountChanged)
    Q_PROPERTY(int tickStep READ tickStep WRITE setTickStep NOTIFY tickStepChanged)
    Q_PROPERTY(int fileLenght READ fileLenght WRITE setFileLenght NOTIFY fileLenghtChanged)
  public:
    QmlMidiFile( QObject *parent = nullptr );

    void configRead( QString fname );

    int  tickCount() const { return mTickCount >> 4; }
    void setTickCount( int tc );

    int  tickStep() const { return mTickStep; }
    void setTickStep( int stp );

    int  fileLenght() const { return mFileLenght; }
    void setFileLenght( int len ) { mFileLenght = len; emit fileLenghtChanged(); }

    Q_INVOKABLE QmlMidiTrack *qmlMidiTrack( int track ) { return mQmlTrack + (track & 0xf); }

    Q_INVOKABLE SvQmlJsonModel *qmlMidiTrackModel() { return &mQmlTrackModel; }
  signals:
    void tickCountChanged();

    void tickStepChanged();

    void fileLenghtChanged();

    void midiEvent( quint8 cmd, quint8 data0, quint8 data1 );

    void voiceSetup( quint8 channel, int voiceId );

  public slots:
    void tick();

    void play();

    void stop();

    bool read( QString fname );

    void configWrite();

  private:
    bool         readMthd( IffReader &reader );
    void         readMtrk( IffReader &reader );
    virtual void readExtension( IffReader &reader );
    quint32      variableLenValue( IffReader &reader );

  };

#endif // QMLMIDIFILE_H
