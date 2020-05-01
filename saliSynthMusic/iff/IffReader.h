/*
 Project "SaliSynth - music synth for linux (raspberry) with midi keyboard support"
 Author:
   Sibilev A.S.
 Web
   SaliLab.com
 Description
   IFF files reader
*/
#ifndef IFFREADER_H
#define IFFREADER_H

#include <QFile>
#include <QByteArray>
#include <QSharedPointer>
#include <functional>

using QFilePtr = QSharedPointer<QFile>;

#define IFF_FOUR( ch0, ch1, ch2, ch3 ) ((static_cast<quint32>(ch0) << 24) | (static_cast<quint32>(ch1) << 16) | (static_cast<quint32>(ch2) << 8) | static_cast<quint32>(ch3))

class IffReader
  {
    QFilePtr mFile;
    char     mChunkName[5];
    quint32  mChunkLength;
    qint64   mChunkStart;
    bool     mRiff;
  public:
    IffReader( const IffReader& ) = default;
    IffReader( QFilePtr file, bool riff );
    IffReader( QString fname, bool riff );
    ~IffReader();

    const char *name() const { return mChunkName; }

    bool        isValid() const { return mChunkLength != 0; }

    bool        isEnd() const;

    bool        needFour( const char *name );

    bool        compareChunkName( const char *name ) const;

    quint32     chunkFour() const { return IFF_FOUR(mChunkName[0],mChunkName[1],mChunkName[2],mChunkName[3]); }

    quint32     chunkLenght() const { return mChunkLength; }

    IffReader   getChunk();

    bool        readChunk( const char *name, std::function<bool( IffReader &reader )> fun1 );

    bool        readChars( char *dst, int len );

    quint8      peekUint8();

    quint8      getUint8();
    quint16     getUint16be();
    quint16     getUint16le();
    quint32     getUint32be();
    quint32     getUint32le();

    qint8       getInt8();
    qint16      getInt16be();
    qint16      getInt16le();
    qint32      getInt32be();
    qint32      getInt32le();
  };

#endif // IFFREADER_H
