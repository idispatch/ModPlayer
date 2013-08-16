#include "Cache.hpp"
#include <QDir>
#include <QFile>
#include <QDebug>

Cache::Cache(QSettings &settings, QObject * parent)
    : QObject(parent),
      m_settings(settings),
      m_maxSize(m_settings.value("cache/maxSize", 100 * 1024 * 1024).toInt()),
      m_maxFiles(m_settings.value("cache/maxFiles", 200).toInt()) {
    initCache();
}

Cache::~Cache() {
    m_settings.beginGroup("cache");
    m_settings.setValue("maxSize", maxSize());
    m_settings.setValue("maxFiles", maxFiles());
    m_settings.endGroup();
}

void Cache::initCache() {
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

void Cache::setFileNameFilters(QStringList const& value) {
    if(value != m_fileNameFilters)
    {
        int oldFiles = currentFiles();
        int oldSize = currentSize();

        m_fileNameFilters = value;
        initCache();

        notifyCacheChanged(oldFiles, oldSize);
    }
}

int Cache::maxSize() const {
    return m_maxSize;
}

int Cache::maxFiles() const {
    return m_maxFiles;
}

int Cache::currentSize() const {
    int totalSize = 0;
    for(int i = 0; i < m_files.size(); i++) {
        totalSize += m_files[i].size();
    }
    return totalSize;
}

int Cache::currentFiles() const {
    return m_files.size();
}

void Cache::setMaxSize(int size) {
    if(m_maxSize != size)
    {
        int oldMaxSize = m_maxSize;
        m_maxSize = size;
        if(oldMaxSize > m_maxSize)
        {
            houseKeep();
        }
        emit maxSizeChanged();
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
        emit maxFilesChanged();
    }
}

void Cache::purge() {
    while(!m_files.empty())
    {
        remove(m_files.first());
    }
}

void Cache::houseKeep() {
    int totalSize = currentSize();
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
            int oldSize = currentSize();

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

        emit currentFilesChanged();
        emit currentSizeChanged();
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
