#ifndef QMLSTYLEFILE_H
#define QMLSTYLEFILE_H

#include "QmlMidiFile.h"

class QmlStyleFile : public QmlMidiFile
  {
    Q_OBJECT
  public:
    QmlStyleFile( QObject *parent = nullptr );

    // QmlMidiFile interface
  private:
    virtual void readExtension(IffReader &reader) override;

    void         readCASM( IffReader &reader );
    void         readCSEG( IffReader &reader );
    void         readOTS_OTSc( IffReader &reader );
    void         readMDB_FNRc( IffReader &reader );
    void         readMH_MHhd( IffReader &reader );
    void         readSdec( IffReader &reader );
    void         readCtab( IffReader &reader );
  };

#endif // QMLSTYLEFILE_H
