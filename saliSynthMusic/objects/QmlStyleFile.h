#ifndef QMLSTYLEFILE_H
#define QMLSTYLEFILE_H

#include "QmlMidiFile.h"

struct StyleTrack {
    quint8 mSrcChannel;
    char   mName[9];
    quint8 mDstChannel;
    quint8 mEditable;
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


class QmlStyleFile : public QmlMidiFile
  {
    Q_OBJECT

    enum StylePart {
      spIntroA  = 0x0001,
      spMainA   = 0x0002,
      spEndingA = 0x0004,
      spIntroB  = 0x0010,
      spMainB   = 0x0020,
      spEndingB = 0x0040,
      spIntroC  = 0x0100,
      spMainC   = 0x0200,
      spEndingC = 0x0400,
      spIntroD  = 0x1000,
      spMainD   = 0x2000,
      spEndingD = 0x4000
      };

    QList<StyleGroup> mGroupList; //! List of segments group
    QSet<QString>     mMarkerSet;
    int               mPart;

    Q_PROPERTY(int parts READ parts NOTIFY partsChanged)
    Q_ENUM(StylePart)
  public:
    QmlStyleFile( QObject *parent = nullptr );

    int parts() const;

  signals:
    void partsChanged();

  public slots:
    void playPart( int part );
//    //!
//    //! \brief intro Play "Intro" and then "Main"
//    //! \param name  Main section name
//    //!
//    void intro( const QString name );

//    //!
//    //! \brief finish After "Main" play "Ending"
//    //!
//    void finish();

//    //!
//    //! \brief section Play "Main" then "Fill In" then "Main"
//    //! \param name    Last Main section name
//    //!
//    void section( const QString name );

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
  };

#endif // QMLSTYLEFILE_H
