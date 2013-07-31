#include "Cache.hpp"
#include <QDir>
#include <QFile>
#include <QDebug>

//#define DEBUG_CACHE

Cache::Cache(QObject * parent)
    : QObject(parent) {
    m_maxSize = 100*1024*1024;
    m_maxFiles = 200;
    initCache();
}

void Cache::initCache() {
#ifdef DEBUG_CACHE
    qDebug() << "Cache::initCache: filters size=" << m_fileNameFilters.size();
#endif
    if(!m_fileNameFilters.empty())
    {
        QString diskPath = cachePath();
        QDir directory(diskPath);
        m_files = directory.entryInfoList(m_fileNameFilters,
                                          QDir::Files,
                                          QDir::Time | QDir::Reversed);
    }
    else
    {
        m_files.clear();
    }
#ifdef DEBUG_CACHE
    qDebug() << "Cache::initCache: files=" << files();
    if(!m_files.empty()) {
        qDebug() << "Cache::initCache: first file=" << m_files.first().absoluteFilePath();
    }
#endif
}

QString Cache::cachePath() const {
    return QDir::homePath();
}

QStringList Cache::files() const {
    QStringList files;
    for(int i = 0; i < m_files.size(); i++) {
        files << m_files[i].fileName();
    }
    return files;
}

QStringList Cache::fileNameFilters() const {
    return m_fileNameFilters;
}

void Cache::notifyCacheChanged(int oldFiles, qint64 oldSize) {
    int newFiles = currentFiles();
    qint64 newSize = currentSize();

    if(oldFiles != newFiles) {
        emit currentFilesChanged(newFiles);
    }
    if(oldSize != newSize) {
        emit currentSizeChanged(newSize);
    }

    if(oldFiles != newFiles || oldSize != newSize) {
        emit filesChanged();
    }
}

void Cache::setFileNameFilters(QStringList const& value) {
    if(value != m_fileNameFilters)
    {
        int oldFiles = currentFiles();
        qint64 oldSize = currentSize();

        m_fileNameFilters = value;
        initCache();

        notifyCacheChanged(oldFiles, oldSize);
    }
}

qint64 Cache::maxSize() const {
    return m_maxSize;
}

int Cache::maxFiles() const {
    return m_maxFiles;
}

qint64 Cache::currentSize() const {
    qint64 totalSize = 0;
    for(int i = 0; i < m_files.size(); i++) {
        totalSize += m_files[i].size();
    }
    return totalSize;
}

int Cache::currentFiles() const {
    return m_files.size();
}

void Cache::setMaxSize(qint64 size) {
    if(m_maxSize != size)
    {
        int oldMaxSize = m_maxSize;
        m_maxSize = size;
        if(oldMaxSize > m_maxSize)
        {
            houseKeep();
        }
        emit maxSizeChanged(m_maxSize);
    }
}

void Cache::setMaxFiles(int size) {
    if(m_maxFiles != size)
    {
        int oldMaxFiles = m_maxFiles;
        m_maxFiles = size;
        if(oldMaxFiles > m_maxFiles)
        {
            houseKeep();
        }
        emit maxFilesChanged(m_maxFiles);
    }
}

void Cache::purge() {
    while(!m_files.empty())
    {
        remove(m_files.first());
    }
}

void Cache::houseKeep() {
    qint64 totalSize = currentSize();
    int totalFiles = currentFiles();
    while(totalFiles > 0 && (totalSize > maxSize() ||
                             totalFiles > maxFiles()))
    {
        QFileInfo const& fileInfo = m_files.first();
#ifdef DEBUG_CACHE
        qDebug() << "Cache::houseKeep: removing " << fileInfo.fileName();
#endif
        remove(fileInfo);
        totalFiles = currentFiles();
        totalSize = currentSize();
    }
}

void Cache::cache(QString const& fileName) {
#ifdef DEBUG_CACHE
    qDebug() << "Cache::cache: caching" << fileName;
#endif
    QString file = absoluteFileName(fileName);
    QFileInfo fileInfo(file);
    if(m_files.indexOf(fileInfo) == -1)
    {
        if(fileInfo.exists()) {
            int oldFiles = currentFiles();
            qint64 oldSize = currentSize();

            m_files.append(fileInfo);
            houseKeep();

            notifyCacheChanged(oldFiles, oldSize);
        }
    }
}

bool Cache::exists(QString const& fileName) {
    bool bExists;
    QString absoluteFilePath = absoluteFileName(fileName);
    QFileInfo fileInfo(absoluteFilePath);
    bExists = m_files.indexOf(fileInfo) != -1 && QFile::exists(absoluteFilePath);
#ifdef DEBUG_CACHE
    qDebug() << "Cache::exists:" << fileName << "(" << absoluteFilePath << ") =" << bExists;
#endif
    return bExists;
}

void Cache::remove(QFileInfo const& fileInfo) {
    remove(fileInfo.fileName());
}

void Cache::remove(QString const& fileName) {
    if(exists(fileName)) {
#ifdef DEBUG_CACHE
        qDebug() << "Cache::remove: removing" << fileName;
#endif
        QString absoluteFilePath = absoluteFileName(fileName);
        QFile::remove(absoluteFilePath);

        QFileInfo fileInfo(absoluteFilePath);
        m_files.removeOne(fileInfo);

        emit currentFilesChanged(currentFiles());
        emit currentSizeChanged(currentSize());
        emit filesChanged();
    }
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
        << ", filesNameFilters=" << c.fileNameFilters()
        << ", cachePath=" << c.cachePath()
        << ")";
    return dbg.space();
}
