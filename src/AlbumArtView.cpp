#include "AlbumArtView.hpp"
#include <bb/cascades/Image>
#include "FileUtils.hpp"
#include "SongFormat.hpp"
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

    if(fileName.isEmpty() || SongFormat::isTrackerSong(fileName))
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

        union ::id3_field* fieldImageEncoding;
        union ::id3_field* fieldImageData;
        unsigned char const * tagBytes;
        ::id3_length_t length;

        fieldImageEncoding = ::id3_frame_field(frame, 1);
        if(fieldImageEncoding == NULL) {
            qDebug() << "Could not parse APIC tag image encoding in file" << fileName;
            break;
        }
        tagBytes = ::id3_field_getlatin1(fieldImageEncoding);
        if(!tagBytes) {
            qDebug() << "Could not get frame field 1 in file" << fileName;
            break;
        }

        if(strncmp(reinterpret_cast<const char*>(tagBytes), "image/", 6)) {
            qDebug() << "MIME type is not supported in file" << fileName;
            break;
        }

        fieldImageData = ::id3_frame_field(frame, 4);
        if(fieldImageData == NULL) {
            qDebug() << "Could not parse APIC tag image data in file" << fileName;
            break;
        }
        tagBytes = ::id3_field_getbinarydata(fieldImageData, &length);
        if (!tagBytes) {
            qDebug() << "Could not get album art data in file" << fileName;
            break;
        }

        if(length > 0 && length < MAX_IMAGE_SIZE) {
            data.append(reinterpret_cast<const char*>(tagBytes), length);
        }

    } while(false);

    if(mp3file != NULL) {
        ::id3_file_close(mp3file);
    }

    if(data.isEmpty()) {
        QString directory = FileUtils::directoryOnly(fileName);

        data = data.isEmpty() ? loadAlbumArtFile(directory) : data;
        data = data.isEmpty() ? loadAlbumArtFile(directory, "folder.jpg") : data;
        data = data.isEmpty() ? loadAlbumArtFile(directory, "folder.png") : data;
        data = data.isEmpty() ? loadAlbumArtFile(directory, "cover.jpg") : data;
        data = data.isEmpty() ? loadAlbumArtFile(directory, "cover.png") : data;
        data = data.isEmpty() ? loadAlbumArtFile(directory, "front.jpg") : data;
        data = data.isEmpty() ? loadAlbumArtFile(directory, "front.png") : data;
        data = data.isEmpty() ? loadSingleImage(directory) : data;
    }

    emit resultReady(data);
}

QByteArray AlbumArtLoader::loadAlbumArtFile(QString const& directory,
                                            QString const& fileName) {
    QString albumArtFile = FileUtils::joinPath(directory, fileName);
    QByteArray data;
    DIR *dirp;
    if ((dirp = ::opendir(directory.toUtf8().constData())) != NULL) {
        struct dirent64 *direntItem;
        do {
            if ((direntItem = ::readdir64(dirp)) != NULL) {
                if(direntItem->d_name[0] == '.') {
                    continue;
                }

                QString currentFileName = QString::fromUtf8(direntItem->d_name);
                if(currentFileName.compare(fileName, Qt::CaseInsensitive) == 0)
                {
                    QString absoluteFileName = FileUtils::joinPath(directory,
                                                                   currentFileName);
                    struct stat64 st;
                    if(0 == ::stat64(absoluteFileName.toUtf8().constData(), &st)) {
                        if((st.st_mode & S_IFREG) && st.st_size < MAX_IMAGE_SIZE) {
                            QFile file(absoluteFileName);
                            if (file.open(QIODevice::ReadOnly)) {
                                data = file.readAll();
                                file.close();
                                break;
                            }
                        }
                    }
                }
            }
        } while (direntItem != NULL);
        ::closedir(dirp);
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
        struct dirent64 *direntItem;
        do {
            if ((direntItem = ::readdir64(dirp)) != NULL) {
                if(direntItem->d_name[0] == '.') {
                    continue;
                }

                QString fileName = QString::fromUtf8(direntItem->d_name);
                QString absoluteFileName = FileUtils::joinPath(directory,
                                                               fileName);
                struct stat64 st;
                if(0 == ::stat64(absoluteFileName.toUtf8().constData(), &st)) {
                    if((st.st_mode & S_IFREG) && st.st_size < MAX_IMAGE_SIZE){
                        if(fileName.startsWith("AlbumArt", Qt::CaseInsensitive) &&
                           fileName.endsWith(".jpg", Qt::CaseInsensitive)) {
                            foundFiles << FileEntry(absoluteFileName, st.st_size);
                        }
                    }
                }
            }
        } while (direntItem != NULL);
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

QByteArray AlbumArtLoader::loadSingleImage(QString const& directory) {
    QByteArray data;
    QList<FileEntry> foundFiles;
    DIR *dirp;

    if ((dirp = ::opendir(directory.toUtf8().constData())) != NULL) {
        struct dirent64 *direntItem;
        do {
            if ((direntItem = ::readdir64(dirp)) != NULL) {
                if(direntItem->d_name[0] == '.') {
                    continue;
                }

                QString fileName = QString::fromUtf8(direntItem->d_name);
                QString absoluteFileName = FileUtils::joinPath(directory,
                                                               fileName);
                struct stat64 st;
                if(0 == ::stat64(absoluteFileName.toUtf8().constData(), &st)) {
                    if(st.st_mode & S_IFREG) {
                        if(fileName.endsWith(".jpg", Qt::CaseInsensitive) ||
                           fileName.endsWith(".png", Qt::CaseInsensitive)) {
                            if(!foundFiles.empty()) {
                                return data;
                            }
                            foundFiles << FileEntry(absoluteFileName, st.st_size);
                        }
                    }
                }
            }
        } while (direntItem != NULL);
        ::closedir(dirp);
    }

    switch(foundFiles.size())
    {
    case 1:
        return loadAlbumArtFile(directory, foundFiles[0].first);
    default:
        break;
    }

    return data;
}

AlbumArtView::AlbumArtView(bb::cascades::Container *parent)
    : bb::cascades::ImageView(parent) {
    AlbumArtLoader *imageLoader = new AlbumArtLoader();
    imageLoader->moveToThread(&m_workerThread);

    bool rc;
    rc = QObject::connect(&m_workerThread, SIGNAL(finished()),
                          imageLoader,     SLOT(deleteLater()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    rc = QObject::connect(this,            SIGNAL(loadAlbumArt(QString)),
                          imageLoader,     SLOT(loadAlbumArt(QString)));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    rc = QObject::connect(imageLoader,     SIGNAL(resultReady(QByteArray)),
                          this,            SLOT(onAlbumArtLoaded(QByteArray)));
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
