#include "AlbumArtView.hpp"
#include <bb/cascades/Image>
#include "FileUtils.hpp"
#include "InstanceCounter.hpp"

#include "libid3tag/id3_id3tag.h"
#include <dirent.h>
#include <errno.h>

template<>
int InstanceCounter<AlbumArtView>::s_count;
template<>
int InstanceCounter<AlbumArtView>::s_maxCount;

void AlbumArtLoader::loadAlbumArt(QString const& fileName) {
    QByteArray data;

    if(fileName.isEmpty())
    {
        emit resultReady(data);
        return;
    }

    ::id3_file * mp3file = ::id3_file_open(fileName.toUtf8().constData(),
                                           ID3_FILE_MODE_READONLY);
    do
    {
        if(mp3file == NULL) {
            qDebug() << "Could not open file" << fileName;
            break;
        }

        ::id3_tag * tag = ::id3_file_tag(mp3file);
        if(tag == NULL) {
            qDebug() << "Could not read ID3 tags from file" << fileName;
            break;
        }

        struct ::id3_frame* frame;

        frame = ::id3_tag_findframe(tag, "APIC", 0);
        if(frame == NULL) {
            qDebug() << "Could not find APIC tag in file" << fileName;
            break;
        }

        union ::id3_field* field;
        unsigned char const * tagBytes;
        ::id3_length_t length;

        field = ::id3_frame_field(frame, 1);
        tagBytes = ::id3_field_getlatin1(field);
        if(!tagBytes) {
            qDebug() << "Could not get frame field 1 in file" << fileName;
            break;
        }

        if(strncmp(reinterpret_cast<const char*>(tagBytes), "image/", 6)) {
            qDebug() << "MIME type is not supported in file" << fileName;
            break;
        }

        field = ::id3_frame_field(frame, 4);
        tagBytes = ::id3_field_getbinarydata(field, &length);
        if (!tagBytes) {
            qDebug() << "Could not get album art data in file" << fileName;
            break;
        }

        if(length > 0) {
            data.append(reinterpret_cast<const char*>(tagBytes), length);
        }

    } while(false);

    if(mp3file != NULL) {
        ::id3_file_close(mp3file);
    }

    if(data.isEmpty()) {
        QString directory = FileUtils::directoryOnly(fileName);

        data = loadAlbumArtFile(directory, "folder.jpg");
        if(data.isEmpty()) {
            data = loadAlbumArtFile(directory, "Folder.jpg");
        }
        if(data.isEmpty()) {
            data = loadAlbumArtFile(directory, "folder.JPG");
        }
        if(data.isEmpty()) {
            data = loadAlbumArtFile(directory, "Folder.JPG");
        }
        if(data.isEmpty()) {
            data = loadAlbumArtFile(directory);
        }
    }

    emit resultReady(data);
}

QByteArray AlbumArtLoader::loadAlbumArtFile(QString const& directory,
                                            QString const& fileName) {
    QString albumArtFile = FileUtils::joinPath(directory, fileName);
    QByteArray data;
    if(FileUtils::exists(albumArtFile)) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            data = file.readAll();
            file.close();
        }
    }
    return data;
}

bool AlbumArtLoader::compareFileSizes(FileEntry const& first,
                                      FileEntry const& second) {
    return first.second > second.second;
}

QByteArray AlbumArtLoader::loadAlbumArtFile(QString const& directory) {
    QByteArray data;
    QList<FileEntry> foundFiles;
    DIR *dirp;
    if ((dirp = ::opendir(directory.toUtf8().constData())) != NULL) {
        struct dirent64 *dp;
        do {
            if ((dp = ::readdir64(dirp)) != NULL) {
                if(dp->d_name[0] == '.' &&
                   (dp->d_name[1] == '\0' ||
                    (dp->d_name[1] == '.' && dp->d_name[2] == '\0'))) {
                    continue;
                }

                QString fileName = QString::fromUtf8(dp->d_name);
                QString absoluteFileName = FileUtils::joinPath(directory,
                                                               fileName);
                struct stat64 st;
                if(0 == ::stat64(absoluteFileName.toUtf8().constData(), &st)) {
                    if(st.st_mode & S_IFREG) {
                        if(fileName.startsWith("AlbumArt") &&
                           (fileName.endsWith(".jpg") ||
                            fileName.endsWith(".JPG"))) {
                            foundFiles << FileEntry(absoluteFileName, st.st_size);
                        }
                    }
                }
            }
        } while (dp != NULL);
        ::closedir(dirp);
    }

    switch(foundFiles.size())
    {
    case 0:
        break;
    case 1:
        return loadAlbumArtFile(directory, foundFiles[0].first);
    default:
        qSort(foundFiles.begin(), foundFiles.end(), compareFileSizes);
        return loadAlbumArtFile(directory, foundFiles[0].first);
    }
    return data;
}

AlbumArtView::AlbumArtView(bb::cascades::Container *parent)
    : bb::cascades::ImageView(parent) {
    AlbumArtLoader *loader = new AlbumArtLoader();
    loader->moveToThread(&m_workerThread);

    bool rc;
    rc = QObject::connect(&m_workerThread, SIGNAL(finished()),
                          loader,         SLOT(deleteLater()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    rc = QObject::connect(this,      SIGNAL(loadAlbumArt(QString)),
                          loader,    SLOT(loadAlbumArt(QString)));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    rc = QObject::connect(loader, SIGNAL(resultReady(QByteArray)),
                          this, SLOT(onAlbumArtLoaded(QByteArray)));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    m_workerThread.start();
}

AlbumArtView::~AlbumArtView() {
    m_workerThread.quit();
    m_workerThread.wait();
}

QString const& AlbumArtView::fileName() const {
    return m_fileName;
}

void AlbumArtView::setFileName(QString const& fileName) {
    if(fileName != m_fileName) {
        m_fileName = fileName;
        emit fileNameChanged();
        emit albumArtLoading();
        loadAlbumArt(m_fileName);
    }
}

void AlbumArtView::onAlbumArtLoaded(QByteArray const& data) {
    setImage(bb::cascades::Image(data));
    setVisible(data.size() > 0);
    if(!data.isEmpty()) {
        emit albumArtLoaded();
    }
}
