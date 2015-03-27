#include "Cache.hpp"
#include <QDir>
#include <QFile>
#include <QDebug>
#include "FileUtils.hpp"
#include "Analytics.hpp"
#include <dirent.h>

//#define VERBOSE_LOGGING

template<>
int InstanceCounter<Cache>::s_count;
template<>
int InstanceCounter<Cache>::s_maxCount;

Cache::Cache(QSettings &settings, QObject * parent)
    : QObject(parent),
      m_settings(settings),
      m_maxSize(m_settings.value("cache/maxSize", 100 * 1024 * 1024).toInt()),
      m_maxFiles(m_settings.value("cache/maxFiles", 200).toInt())
{
}

Cache::~Cache() {
    m_settings.beginGroup("cache");
    m_settings.setValue("maxSize", maxSize());
    m_settings.setValue("maxFiles", maxFiles());
    m_settings.endGroup();
#ifdef VERBOSE_LOGGING
    qDebug() << "Cache::~Cache()";
#endif
}

bool Cache::fileMatches(QString const& fileName) const {
    QString const& extension = FileUtils::extension(fileName);
    return m_filters.contains(extension, Qt::CaseInsensitive);
}

void Cache::initCache() {
    int oldFiles = currentFiles();
    int oldSize = currentSize();

    if(!m_filters.empty())
    {
        QString path = cachePath();
        DIR *dirp;
        if ((dirp = ::opendir(path.toUtf8().constData())) != NULL) {
            struct dirent64 *direntItem;
            do {
                if ((direntItem = ::readdir64(dirp)) != NULL) {
                    if(direntItem->d_name[0] == '.') {
                        continue;
                    }
                    QString fileName = QString::fromUtf8(direntItem->d_name);
                    QString absolute = FileUtils::joinPath(path, fileName);
                    struct stat64 st;
                    if(0 == ::stat64(absolute.toUtf8().constData(), &st)) {
                        if(st.st_mode & S_IFREG) {
                            if(fileMatches(fileName)) {
                                CacheFileInfo info(st.st_size, st.st_ctime, fileName);
                                m_files[fileName] = info;
                                m_sortedFiles.insert(info);
                            }
                        }
                    }
                }
            } while (direntItem != NULL);
            ::closedir(dirp);
        }
    }
    else
    {
        m_files.clear();
        m_sortedFiles.clear();
    }
#ifdef VERBOSE_LOGGING
    qDebug() << "Cache: filters=" << m_filters;
    qDebug() << "Cache: files=" << m_files.size() << ":" << m_sortedFiles.size();
    qDebug() << "Cache: used=" << currentSize();
    for(ConstSetIterator i = m_sortedFiles.begin(); i != m_sortedFiles.end(); ++i) {
        qDebug() << *i;
    }
#endif
    notifyCacheChanged(oldFiles, oldSize);
}

QString Cache::cachePath() const {
    return QDir::homePath();
}

QStringList Cache::files() const {
    QStringList files;
    for(ConstMapIterator i = m_files.begin(); i!= m_files.end(); ++i) {
        files << i->second.name();
    }
    return files;
}

QStringList const& Cache::filters() const {
    return m_filters;
}

void Cache::notifyCacheChanged(int oldFiles, int oldSize) {
    int newFiles = currentFiles();
    int newSize = currentSize();

    if(oldFiles != newFiles) {
        emit currentFilesChanged();
    }
    if(oldSize != newSize) {
        emit currentSizeChanged();
    }

    if(oldFiles != newFiles || oldSize != newSize) {
        emit filesChanged();
    }
}

void Cache::setFilters(QStringList const& filters) {
    m_filters = filters;
}

int Cache::maxSize() const {
    return m_maxSize;
}

int Cache::maxFiles() const {
    return m_maxFiles;
}

int Cache::currentSize() const {
    qint64 totalSize = 0;
    for(ConstMapIterator i = m_files.begin(); i != m_files.end(); ++i) {
        totalSize += i->second.size();
    }
    return totalSize;
}

int Cache::currentFiles() const {
    return m_files.size();
}

void Cache::setMaxSize(int size) {
    if(size < 0) {
        size = 0;
    }
    if(m_maxSize != size) {
        int oldMaxSize = m_maxSize;
        m_maxSize = size;
        if(oldMaxSize > m_maxSize) {
            checkOverflow();
        }
        emit maxSizeChanged();
    }
}

void Cache::setMaxFiles(int size) {
    if(size < 0) {
        size = 0;
    }
    if(m_maxFiles != size) {
        int oldMaxFiles = m_maxFiles;
        m_maxFiles = size;
        if(oldMaxFiles > m_maxFiles) {
            checkOverflow();
        }
        emit maxFilesChanged();
    }
}

void Cache::purge() {
    while(!m_files.empty()) {
        removeByName(m_files.begin()->first);
    }
}

void Cache::checkOverflow() {
    int totalSize = currentSize();
    int totalFiles = currentFiles();
    while(!m_sortedFiles.empty() &&
          (totalSize > maxSize() || totalFiles > maxFiles()))
    {
        CacheFileInfo info = *m_sortedFiles.begin();
        removeByName(info.name());
        totalFiles = currentFiles();
        totalSize  = currentSize();
    }
}

void Cache::cache(QString const& fileName) {
    if(FileUtils::isRelative(fileName)) {
        if(m_files.find(fileName) != m_files.end()) {
            int oldFiles = currentFiles();
            int oldSize = currentSize();
            struct stat64 st;
            QString absoluteFileNameStr = absoluteFileName(fileName);
            int rc = ::stat64(absoluteFileNameStr.toUtf8().constData(), &st);
            if(rc == 0) {
                CacheFileInfo info(st.st_size, st.st_ctime, fileName);
                m_files[fileName] = info;
                m_sortedFiles.insert(info);
                checkOverflow();
                notifyCacheChanged(oldFiles, oldSize);
            }
        }
    }
}

bool Cache::fileExists(QString const& fileName) {
    if(FileUtils::isAbsolute(fileName)) {
        return FileUtils::fileExists(fileName);
    } else {
        return FileUtils::fileExists(FileUtils::joinPath(cachePath(), fileName));
    }
}

void Cache::removeByName(QString const& fileName) {
    if(FileUtils::isRelative(fileName)) {
        if(fileExists(fileName)) {
            QFile::remove(absoluteFileName(fileName));
            ConstMapIterator i = m_files.find(fileName);
            if(i != m_files.end()) {
#ifdef VERBOSE_LOGGING
                qDebug() << "Cache: removing" << i->second;
#endif
                m_sortedFiles.erase(i->second);
                m_files.erase(i->first);
            }
            emit currentFilesChanged();
            emit currentSizeChanged();
            emit filesChanged();
        }
    }
}

void Cache::save(QString const& cacheFileName, QString const& newFileName) {
    if(fileExists(cacheFileName))
    {
        QString absoluteCacheFileName = absoluteFileName(cacheFileName);
        bool copyOk;
        if(FileUtils::fileExists(newFileName))
        {
            if(QFile::remove(newFileName)) {
                copyOk = true;
            } else {
                copyOk = false;
            }
        } else {
            copyOk = true;
        }
        if(copyOk) {
            QFile::copy(absoluteCacheFileName, newFileName);
            if(!FileUtils::adjustPermissions(newFileName)) {
                qDebug() << "Failed to set permissions for file " << newFileName;
            }
            Analytics::getInstance()->saveCache(cacheFileName, newFileName);
        }
    }
}

void Cache::exportCache(QString const& directory) {
    int numFiles = currentFiles();
    int numCopiedFiles = 0;

    QString targetDirectory;
    if(FileUtils::isAbsolute(directory))
    {
        targetDirectory = directory;
    }
    else
    {
        targetDirectory = FileUtils::joinPath("/accounts/1000/shared/downloads", directory);
    }

    if(QDir().mkpath(targetDirectory))
    {
        int currentCount = 0;
        for(ConstMapIterator i = m_files.begin(); i != m_files.end(); ++i)
        {
            QString destinationFileName(FileUtils::joinPath(targetDirectory, i->second.name()));

            if(FileUtils::fileExists(destinationFileName))
            {
                if(!QFile::remove(destinationFileName))
                {
                    qDebug() << "Failed to remove" << destinationFileName;
                    continue;
                }
                else
                {
                    qDebug() << "Removed" << destinationFileName;
                }
            }

            QString sourceFileName(absoluteFileName(i->second.name()));

            currentCount += 1;
            emit progressUpdate(currentCount * 100 / numFiles, i->second.name());

            if(QFile::copy(sourceFileName, destinationFileName))
            {
                qDebug() << "Copied from" << i->second.name() << "to" << destinationFileName;
                if(!FileUtils::adjustPermissions(destinationFileName)) {
                    qDebug() << "Failed to set permissions for file " << destinationFileName;
                }
                numCopiedFiles += 1;
            }
            else
            {
                qDebug() << "Failed to copy from" << sourceFileName << "to" << destinationFileName;
                continue;
            }
        }
    }
    else {
        Analytics::getInstance()->logError("Failed to create cache export path", targetDirectory);
    }

    Analytics::getInstance()->exportCache(numFiles, numCopiedFiles);
}

QString Cache::absoluteFileName(QString const& fileName) const {
    if(fileName.startsWith('/')) {
        return fileName;
    } else {
        return cachePath() + "/" + fileName;
    }
}

QDebug operator << (QDebug dbg, Cache const &c) {
    dbg.nospace()
        << "(Cache: currentFiles=" << c.currentFiles()
        << ", currentSize=" << c.currentSize()
        << ", maxFiles=" << c.maxFiles()
        << ", maxSize=" << c.maxSize()
        << ", files=" << c.files()
        << ", filters=" << c.filters()
        << ", cachePath=" << c.cachePath()
        << ")";
    return dbg.space();
}
