#ifndef ALBUMARTVIEW_H_
#define ALBUMARTVIEW_H_

#include <QObject>
#include <QMetaType>
#include <QThread>
#include <bb/cascades/ImageView>
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

class AlbumArtView : public bb::cascades::ImageView,
                     public InstanceCounter<AlbumArtView> {
    Q_OBJECT
    Q_DISABLE_COPY(AlbumArtView)
    Q_PROPERTY (QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
public:
    AlbumArtView(bb::cascades::Container * parent = 0);
    ~AlbumArtView();
    QString const& fileName() const;
signals:
    void fileNameChanged();
    void loadAlbumArt(QString const& fileName);
    void albumArtLoading();
    void albumArtLoaded();
public slots:
    void setFileName(QString const& fileName);
    void onAlbumArtLoaded(QByteArray const& data);
private:
    QThread m_workerThread;
    QString m_fileName;
};

Q_DECLARE_METATYPE(AlbumArtView*);

class AlbumArtLoader : public QObject
{
    Q_OBJECT
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

#endif
