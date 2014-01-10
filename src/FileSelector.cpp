#include <dirent.h>
#include <errno.h>
#include "FileSelector.hpp"
#include "FileUtils.hpp"
#include <QDir>
#include <QDebug>
#include <QThread>
#include <QStack>

FileSelector::FileSelector(QStringList const &filters) {
    m_filters.reserve(filters.size());
    std::transform(filters.begin(), filters.end(),
                   std::back_inserter(m_filters),
                   createExtensionFilter);
}

FileSelector::~FileSelector() {
#ifdef VERBOSE_LOGGING
    qDebug() << "FileSelector::~FileSelector()";
#endif
}

QString FileSelector::createExtensionFilter(QString const& p) {
    return p.mid(1);
}

bool FileSelector::isMp3(QString const& fileName) {
    return fileName.endsWith(".mp3", Qt::CaseInsensitive);
}

bool FileSelector::fileMatches(QString const& fileName) {
    foreach(QString const& filter, m_filters) {
        if(fileName.endsWith(filter, Qt::CaseInsensitive))
            return true;
    }
    return false;
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
        "removable/sdcard/music/Dimitri from Paris"
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
    for(size_t i = 0; i < sizeof(locations)/sizeof(locations[0]); ++i) {
        scanDirectory(locations[i]);
    }
    emit done();
}

void FileSelector::scanDirectory(const char * path) {
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
                    if(dp->d_name[0] == '.' && (dp->d_name[1] == '\0' || (dp->d_name[1] == '.' && dp->d_name[2] == '\0'))) {
                        continue;
                    }

                    QString absoluteFileName = FileUtils::joinPath(directoryPath, QString::fromUtf8(dp->d_name));
                    struct stat64 st;
                    if(0 == ::stat64(absoluteFileName.toUtf8().constData(), &st)) {
                        if(st.st_mode & S_IFDIR) {
                            stack.push(absoluteFileName);
                        } else {
                            if(st.st_mode & S_IFREG) {
                                emit foundFile(absoluteFileName);
                            }
                        }
                    }
                }
            } while (dp != NULL);
            ::closedir(dirp);
        } else {
            qDebug() << "Failed to open directory" << directoryPath;
        }
   }
}

