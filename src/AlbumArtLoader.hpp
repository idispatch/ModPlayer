#ifndef ALBUMARTLOADER_HPP
#define ALBUMARTLOADER_HPP

#include <QObject>
#include <QMetaType>
#include <QPair>
#include <QString>
#include <QByteArray>

#include "InstanceCounter.hpp"

#include "taglib/taglib.h"
#include "taglib/fileref.h"

#include "taglib/tag.h"
#include "taglib/apetag.h"
#include "taglib/id3v2tag.h"

#include "taglib/mpcfile.h"
#include "taglib/mp4file.h"
#include "taglib/mpegfile.h"
#include "taglib/apefile.h"
#include "taglib/asffile.h"
#include "taglib/flacfile.h"
#include "taglib/vorbisfile.h"
#include "taglib/wavpackfile.h"

class AlbumArtLoader : public QObject,
                       public InstanceCounter<AlbumArtLoader>
{
    Q_OBJECT
    Q_DISABLE_COPY(AlbumArtLoader)
public:
    enum {
        MAX_IMAGE_SIZE = 4*1024*1024 // 4 MB
    };
    AlbumArtLoader(QObject * parent = 0)
        : QObject(parent)
    {}
public slots:
    void loadAlbumArt(QString const& fileName);
signals:
    void resultReady(QByteArray const& result);
private:
    typedef QPair<QString, off64_t> FileEntry;

    QByteArray loadAlbumArtFile(QString const& directory,
                                QString const& fileName);

    static bool extractAPE(TagLib::APE::Tag* tag, QByteArray& target);
    static bool extractID3(TagLib::ID3v2::Tag* tag, QByteArray& target);
    static bool extractASF(TagLib::ASF::File* file, QByteArray& target);
    static bool extractFLAC(TagLib::FLAC::File* file, QByteArray& target);
    static bool extractMP4(TagLib::MP4::File* file, QByteArray& target);

    QByteArray loadAlbumArtFile(QString const& directory);
    QByteArray loadSingleImage(QString const& directory);

    static bool compareFileSizes(FileEntry const& first,
                                 FileEntry const& second);
};

Q_DECLARE_METATYPE(AlbumArtLoader*);

#endif
