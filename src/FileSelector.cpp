#include <dirent.h>
#include <errno.h>
#include "FileSelector.hpp"
#include "FileUtils.hpp"
#include <QDir>
#include <QDebug>
#include <QThread>
#include <QStack>
#include <qtextcodec.h>

#ifdef _DEBUG
//#define VERBOSE_LOGGING
//#define REDUCED_SEARCH_SCOPE
#else
#endif

FileSelector::FileSelector(QStringList const &filters) {
    m_filters.reserve(filters.size());
    std::transform(filters.begin(), filters.end(),
                   std::back_inserter(m_filters),
                   createExtensionFilter);
#ifdef VERBOSE_LOGGING
    qDebug() << "Filters:";
    foreach(QString const& item, m_filters) {
        qDebug() << item;
    }
#endif
}

FileSelector::~FileSelector() {
#ifdef VERBOSE_LOGGING
    qDebug() << "FileSelector::~FileSelector()";
#endif
}

QString FileSelector::createExtensionFilter(QString const& p) {
    if(p.startsWith(QChar('*'))) {
        return p.mid(1); // remove star from file extension
    }
    return p;
}

bool FileSelector::isMp3(QString const& fileName) {
    return fileName.endsWith(".mp3", Qt::CaseInsensitive);
}

bool FileSelector::isPlaylist(QString const& fileName) {
    return fileName.endsWith(".m3u", Qt::CaseInsensitive) ||
           fileName.endsWith(".m3u8", Qt::CaseInsensitive);
}

bool FileSelector::fileMatches(QString const& fileName) const {
    QString const& extension = FileUtils::extension(fileName);
    return m_filters.contains(extension, Qt::CaseInsensitive);
}

bool FileSelector::playlistMatches(QString const& fileName) const {
    return isPlaylist(fileName);
}

void FileSelector::processPlaylist(QString const& playlist,
                                   QSet<QString> const& foundFiles) {
    Q_UNUSED(foundFiles);
    QVector<QString> playlistFiles;
#ifdef VERBOSE_LOGGING
    qDebug() << "Processing playlist file" << playlist;
#endif
    const QString extension = FileUtils::extension(playlist);
    const QString name = FileUtils::fileNameOnlyWithoutExtension(playlist);
    const QString directory = FileUtils::directoryOnly(playlist);

    QFile playlistFile(playlist);
    if(!playlistFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open playlist" << playlist;
    }
#ifdef VERBOSE_LOGGING
    qDebug() << "Processing playlist" << name << "in directory" << directory;
#endif
    int missingSongs = 0;
    QTextStream textStream(&playlistFile);
    if(extension.endsWith(".m3u", Qt::CaseInsensitive)) {
        textStream.setCodec("ISO 8859-1");
    } else {
        textStream.setCodec("UTF-8");
        textStream.setAutoDetectUnicode(true);
    }
#ifdef VERBOSE_LOGGING
    qDebug() << "Using codec:" << textStream.codec()->name().constData();
#endif
    while(!textStream.atEnd())
    {
       QString line = textStream.readLine();
       if (line.isEmpty() || line[0] == '#' || line.size() > 4096)
           continue;

       QString const fullSongPath = FileUtils::joinPath(directory, line);
       if(!isMp3(fullSongPath)) {
#ifdef VERBOSE_LOGGING
           qDebug() << "Skipping non-mp3 song" << fullSongPath;
#endif
           continue;
       }
       if(FileUtils::exists(fullSongPath)) {
           if(foundFiles.contains(fullSongPath)) {
               playlistFiles << fullSongPath;
           } else {
#ifdef VERBOSE_LOGGING
               qDebug() << "Song" << fullSongPath << "was not found in scan";
#endif
               missingSongs++;
           }
       } else {
#ifdef VERBOSE_LOGGING
           qDebug() << "Song" << fullSongPath << "is not in the file system";
#endif
       }
    }

    if(!playlistFiles.empty()) {
#ifdef VERBOSE_LOGGING
        qDebug() << "Playlist" << name
                 << "has" << playlistFiles.size()
                 << "songs and" << missingSongs << "missing";
#endif
        emit foundPlaylist(name, playlistFiles);
    }
}

void FileSelector::processPlaylists(QSet<QString> const& playlists,
                                    QSet<QString> const& foundFiles) {
#ifdef VERBOSE_LOGGING
    qDebug() << "Found files:" << foundFiles.size();
    foreach(QString const& songPath, foundFiles) {
        qDebug() << songPath;
    }
    qDebug() << "Processing" << playlists.size() << "playlists";
#endif
    foreach(QString const& playlist, playlists) {
        processPlaylist(playlist, foundFiles);
    }
}

void FileSelector::start() {
    bool rc;
    Q_UNUSED(rc);

    QThread * thread = new QThread();
    moveToThread(thread);

    rc = QObject::connect(thread, SIGNAL(started()),
                          this,   SLOT(selectFiles()));
    Q_ASSERT(rc);

    rc = QObject::connect(thread, SIGNAL(finished()),
                          this,   SLOT(deleteLater()));
    Q_ASSERT(rc);

    rc = QObject::connect(thread, SIGNAL(finished()),
                          thread, SLOT(deleteLater()));
    Q_ASSERT(rc);

    thread->start(QThread::HighPriority);
}

void FileSelector::selectFiles() {
#ifdef REDUCED_SEARCH_SCOPE
    static const char * locations[] = {
        "/accounts/1000/shared/music"
    };
#else
    static const char * locations[] = {
        "/accounts/1000/shared/documents",
        "/accounts/1000/shared/downloads",
        "/accounts/1000/shared/music",
        "/accounts/1000/shared/Box",
        "/accounts/1000/shared/Dropbox",
        "/accounts/1000/removable/sdcard"
    };
#endif
    QSet<QString> foundFiles;
    QSet<QString> foundPlaylists;
    for(size_t i = 0; i < sizeof(locations)/sizeof(locations[0]); ++i) {
        scanDirectory(locations[i], foundFiles, foundPlaylists);
    }
    if(!foundPlaylists.empty()) {
        processPlaylists(foundPlaylists, foundFiles);
    }
    emit done();
}

void FileSelector::scanDirectory(const char * path,
                                 QSet<QString>& foundFiles,
                                 QSet<QString>& foundPlaylists) {
    QStack<QString> stack;
    stack.push(path);
    while(!stack.isEmpty()) {
        QString directoryPath = stack.pop();
        QString locationDisplay = directoryPath;
        locationDisplay.remove(0, 15);
        emit searchingDirectory(locationDisplay);
        QDir directory(directoryPath);
#if 0
        // This does not work because file system uses 64 bit inodes
        QStringList entries = directory.entryList(m_filters,
                                                  QDir::Files | QDir::Readable | QDir::CaseSensitive,
                                                  QDir::NoSort);
#endif
        DIR *dirp;
        if ((dirp = ::opendir(directoryPath.toUtf8().constData())) != NULL) {
            struct dirent64 *dp;
            do {
                if ((dp = ::readdir64(dirp)) != NULL) {
                    // Skip current directory, parent directory and dot files
                    if(dp->d_name[0] == '.') {
                        continue;
                    }

                    QString absoluteFileName = FileUtils::joinPath(directoryPath,
                                                                   QString::fromUtf8(dp->d_name));
                    struct stat64 st;
                    if(0 == ::stat64(absoluteFileName.toUtf8().constData(), &st)) {
                        if(st.st_mode & S_IFDIR) {
                            stack.push(absoluteFileName);
                        } else {
                            if(st.st_mode & S_IFREG) {
                                if(playlistMatches(absoluteFileName)) {
                                    foundPlaylists << absoluteFileName;
                                }
                                if(fileMatches(absoluteFileName)) {
                                    foundFiles << absoluteFileName;
                                    emit foundFile(absoluteFileName);
                                }
                            }
                        }
                    }
                }
            } while (dp != NULL);
            ::closedir(dirp);
        }
   }
}

