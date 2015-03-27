#include "AlbumArtView.hpp"
#include <bb/cascades/Image>
#include "FileUtils.hpp"
#include "SongFormat.hpp"
#include "InstanceCounter.hpp"

#include "taglib/attachedpictureframe.h"
#include "taglib/commentsframe.h"
#include "taglib/id3v1genres.h"
#include "taglib/textidentificationframe.h"
#include "taglib/tstring.h"

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

    // http://rainmeter.googlecode.com/svn/trunk/Plugins/PluginNowPlaying/

    bool found = false;
    TagLib::FileRef f(fileName.toUtf8().constData(),
                      true,
                      TagLib::AudioProperties::Accurate);
    if(TagLib::MPEG::File* file = dynamic_cast<TagLib::MPEG::File*>(f.file()))
    {
        if(file->ID3v2Tag())
        {
            found = extractID3(file->ID3v2Tag(), data);
        }
        if(!found && file->APETag())
        {
            found = extractAPE(file->APETag(), data);
        }
    }
    else if(TagLib::MP4::File* file = dynamic_cast<TagLib::MP4::File*>(f.file()))
    {
        if(file->tag())
        {
            found = extractMP4(file, data);
        }
    }
    else if(TagLib::FLAC::File* file = dynamic_cast<TagLib::FLAC::File*>(f.file()))
    {
        found = extractFLAC(file, data);
        if(!found && file->ID3v2Tag())
        {
            found = extractID3(file->ID3v2Tag(), data);
        }
    }
    else if(TagLib::ASF::File* file = dynamic_cast<TagLib::ASF::File*>(f.file()))
    {
        found = extractASF(file, data);
    }
    else if(TagLib::APE::File* file = dynamic_cast<TagLib::APE::File*>(f.file()))
    {
        if(file->APETag())
        {
            found = extractAPE(file->APETag(), data);
        }
    }
    else if(TagLib::MPC::File* file = dynamic_cast<TagLib::MPC::File*>(f.file()))
    {
        if(file->APETag())
        {
            found = extractAPE(file->APETag(), data);
        }
    }
    else if(TagLib::WavPack::File* file = dynamic_cast<TagLib::WavPack::File*>(f.file()))
    {
        if(file->APETag())
        {
            found = extractAPE(file->APETag(), data);
        }
    }

    if(!found || data.isEmpty()) {
        QString directory = FileUtils::directoryOnly(fileName);

        do
        {
            data = loadAlbumArtFile(directory);
            if(!data.isEmpty())
                break;

            const char * candidates[] = {
                "folder.jpeg",
                "folder.jpg",
                "folder.png",

                "cover.jpeg",
                "cover.jpg",
                "cover.png",

                "cd.jpeg",
                "cd.jpg",
                "cd.png",

                "front.jpeg",
                "front.jpg",
                "front.png",

                "back.jpeg",
                "back.jpg",
                "back.png",

                "inside.jpeg",
                "inside.jpg",
                "inside.png",

                "inlay.jpeg",
                "inlay.jpg",
                "inlay.png",
            };
            const size_t numCandidates = sizeof(candidates)/sizeof(candidates[0]);

            for(size_t i = 0; i < numCandidates && data.isEmpty(); ++i) {
                data = loadAlbumArtFile(directory, candidates[i]);
            }

            if(!data.isEmpty()) {
                break;
            } else {
                const char * subDirs[] = {
                    "Covers",
                    "covers",
                    "Cover",
                    "cover",
                    "CD",
                    "cd",
                    "Artworks",
                    "artworks",
                    "AlbumArt",
                    "Extras",
                    "Scans",
                    "scans",
                    "Misc",
                    "misc",
                    "Art",
                    "art"
                };
                const size_t numSubDirs = sizeof(subDirs)/sizeof(subDirs[0]);
                for(size_t j = 0; j < numSubDirs && data.isEmpty(); ++j) {
                    const QString coversDirectory = FileUtils::joinPath(directory, subDirs[j]);
                    if(FileUtils::directoryExists(coversDirectory)) {
                        for(size_t i = 0; i < numCandidates && data.isEmpty(); ++i) {
                            data = loadAlbumArtFile(coversDirectory, candidates[i]);
                        }
                    }
                }
            }

            if(!data.isEmpty())
                break;

            data = loadSingleImage(directory);
            if(!data.isEmpty())
                break;

        } while(false);
    }

    emit resultReady(data);
}

bool AlbumArtLoader::extractAPE(TagLib::APE::Tag* tag, QByteArray& target)
{
    bool rc = false;
    const TagLib::APE::ItemListMap& listMap = tag->itemListMap();
    if(listMap.contains("COVER ART (FRONT)"))
    {
        const TagLib::ByteVector nullStringTerminator(1, 0);

        TagLib::ByteVector item = listMap["COVER ART (FRONT)"].value();
        int pos = item.find(nullStringTerminator);  // Skip the filename

        if(++pos > 0)
        {
            const TagLib::ByteVector& pic = item.mid(pos);
            target.append(pic.data(), pic.size());
            rc =  true;
        }
    }

    return rc;
}

// Extracts cover art embedded in ID3v2 tags.
bool AlbumArtLoader::extractID3(TagLib::ID3v2::Tag* tag, QByteArray& target)
{
    bool rc = false;
    const TagLib::ID3v2::FrameList& frameList = tag->frameList("APIC");
    if(!frameList.isEmpty())
    {
        // Grab the first image
        TagLib::ID3v2::AttachedPictureFrame* frame =
                static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frameList.front());
        if(frame) {
            target.append(frame->picture().data(), frame->picture().size());
            rc =  true;
        }
    }

    return rc;
}

// Extracts cover art embedded in ASF/WMA files.
bool AlbumArtLoader::extractASF(TagLib::ASF::File* file, QByteArray& target)
{
    bool rc = false;
    const TagLib::ASF::AttributeListMap& attrListMap = file->tag()->attributeListMap();
    if(attrListMap.contains("WM/Picture"))
    {
        const TagLib::ASF::AttributeList& attrList = attrListMap["WM/Picture"];
        if(!attrList.isEmpty())
        {
            // Let's grab the first cover. TODO: Check/loop for correct type
            TagLib::ASF::Picture wmpic = attrList[0].toPicture();
            if(wmpic.isValid())
            {
                target.append(wmpic.picture().data(), wmpic.picture().size());
                rc =  true;
            }
        }
    }

    return rc;
}

// Extracts cover art embedded in FLAC files.
bool AlbumArtLoader::extractFLAC(TagLib::FLAC::File* file, QByteArray& target)
{
    bool rc = false;
    const TagLib::List<TagLib::FLAC::Picture*>& picList = file->pictureList();
    if(!picList.isEmpty())
    {
        // Let's grab the first image
        TagLib::FLAC::Picture* pic = picList[0];
        target.append(pic->data().data(), pic->data().size());
        rc = true;
    }

    return rc;
}

// Extracts cover art embedded in MP4-like files.
bool AlbumArtLoader::extractMP4(TagLib::MP4::File* file, QByteArray& target)
{
    bool rc = false;
    TagLib::MP4::Tag* tag = file->tag();
    if(tag->itemListMap().contains("covr"))
    {
        TagLib::MP4::CoverArtList coverList = tag->itemListMap()["covr"].toCoverArtList();
        if(coverList[0].data().size() > 0)
        {
            target.append(coverList[0].data().data(), coverList[0].data().size());
            rc = true;
        }
    }

    return rc;
}

QByteArray AlbumArtLoader::loadAlbumArtFile(QString const& directory,
                                            QString const& fileName) {
    QString albumArtFile = FileUtils::joinPath(directory, fileName);
    QByteArray data;
    DIR *dirp;
    if((dirp = ::opendir(directory.toUtf8().constData())) != NULL) {
        struct dirent64 *direntItem;
        do {
            if((direntItem = ::readdir64(dirp)) != NULL) {
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
                        if(S_ISREG(st.st_mode) && st.st_size <= MAX_IMAGE_SIZE) {
                            QFile file(absoluteFileName);
                            if(file.open(QIODevice::ReadOnly)) {
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
    if((dirp = ::opendir(directory.toUtf8().constData())) != NULL) {
        struct dirent64 *direntItem;
        do {
            if((direntItem = ::readdir64(dirp)) != NULL) {
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

    if((dirp = ::opendir(directory.toUtf8().constData())) != NULL) {
        struct dirent64 *direntItem;
        do {
            if((direntItem = ::readdir64(dirp)) != NULL) {
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
                           fileName.endsWith(".jpeg", Qt::CaseInsensitive) ||
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

    m_workerThread.start(QThread::LowPriority);
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
