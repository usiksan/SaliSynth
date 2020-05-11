#ifndef QMLSTYLEFILE_H
#define QMLSTYLEFILE_H

#include "QmlMidiFile.h"

#include <QQueue>

struct StyleTrack {
    quint8  mSrcChannel;
    char    mName[9];
    quint8  mDstChannel;
    quint8  mEditable;
    quint16 mNoteMuteMask;
    quint64 mChordMute;
    quint8  mSourceChord;
    quint8  mSourceChordType;
    quint8  mNoteTranspositionRules;
    quint8  mNoteTranspositionTable;
    quint8  mHighKey;
    quint8  mNoteLowLimit;
    quint8  mNoteHighLimit;
    quint8  mTriggerRule;
  };

struct StyleGroup {
    QSet<QString>     mMarkerSet;  //! Set of markers for which segments this group
    QList<StyleTrack> mTrackList;  //! Track info list for segments of group
  };


struct StyleLoop {
    qint32      mTimeStart;
    qint32      mTimeStop;
    int         mPartMask;
    bool        mLoop;
    StyleGroup *mGroup;
  };

class QmlStyleFile : public QmlMidiFile
  {
    Q_OBJECT

    QList<StyleGroup> mGroupList; //! List of segments group
    QSet<QString>     mMarkerSet;
    int               mCurPart;   //! Current style part
    int               mMainPart;  //! Active main part

    QQueue<StyleLoop> mLoop;
    quint8            mNote;
    quint8            mChordType;

    Q_PROPERTY(int parts READ parts NOTIFY partsChanged)
    Q_PROPERTY(int curPart READ curPart NOTIFY curPartChanged)
    Q_PROPERTY(int mainPart READ mainPart NOTIFY mainPartChanged)

  public:
    enum StylePart {
      spIntroA  = 0x0001,
      spMainA   = 0x0002,
      spEndingA = 0x0004,
      spFillA   = 0x0008,
      spIntroB  = 0x0010,
      spMainB   = 0x0020,
      spEndingB = 0x0040,
      spFillB   = 0x0080,
      spIntroC  = 0x0100,
      spMainC   = 0x0200,
      spEndingC = 0x0400,
      spFillC   = 0x0800,
      spIntroD  = 0x1000,
      spMainD   = 0x2000,
      spEndingD = 0x4000,
      spFillD   = 0x8000
      };

    Q_ENUM(StylePart)
  public:
    QmlStyleFile( QObject *parent = nullptr );

    int parts() const;

    int  curPart() const { return mCurPart; }

    int  mainPart() const { return mMainPart; }

  signals:
    void partsChanged();
    void curPartChanged();
    void mainPartChanged();

  public slots:
    void playPart( int part );

    virtual void tick() override;

    void         chord( quint8 note, quint8 chordType );

    // QmlMidiFile interface
  private:
    virtual void readExtension(IffReader &reader) override;

    void         readCASM( IffReader &reader );
    void         readCSEG( IffReader &reader );
    void         readOTS_OTSc( IffReader &reader );
    void         readMDB_FNRc( IffReader &reader );
    void         readMH_MHhd( IffReader &reader );
    void         readSdec(IffReader &reader , StyleGroup &group);
    void         readCtab( IffReader &reader, StyleGroup &group );

    // QmlMidiFile interface
  private:
    virtual void postRead() override;

    void         addPart(int partMask, const QString part, bool loop );

    void         addMainPart( int mainPart );
  };

#endif // QMLSTYLEFILE_H
