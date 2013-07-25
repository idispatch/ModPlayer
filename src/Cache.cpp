#include "Cache.hpp"
#include <QDir>
#include <QFile>

Cache::Cache(QObject * parent)
    : QObject(parent) {
    m_maxSize = 50*1024*1024;
    m_maxFiles = 100;
    initCache();
}

void Cache::initCache() {
    if(!m_fileNameFilters.empty()) {
        QString diskPath = QDir::homePath();
        QDir directory(diskPath);
        m_files = directory.entryInfoList(m_fileNameFilters,
                                          QDir::Files,
                                          QDir::Time | QDir::Reversed);
    } else {
        m_files.clear();
    }
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
    if(value != m_fileNameFilters) {
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
    if(m_maxSize != size) {
        int oldMaxSize = m_maxSize;
        m_maxSize = size;
        if(oldMaxSize > m_maxSize) {
            houseKeep();
        }
        emit maxSizeChanged(m_maxSize);
    }
}

void Cache::setMaxFiles(int size) {
    if(m_maxFiles != size) {
        int oldMaxFiles = m_maxFiles;
        m_maxFiles = size;
        if(oldMaxFiles > m_maxFiles) {
            houseKeep();
        }
        emit maxFilesChanged(m_maxFiles);
    }
}

void Cache::purge() {
    while(!m_files.empty()) {
        remove(m_files[0].fileName());
    }
}

void Cache::houseKeep() {
    qint64 totalSize = currentSize();
    int totalFiles = currentFiles();
    while(totalFiles > 0 && (totalSize > maxSize() ||
                             totalFiles > maxFiles())) {
        remove(m_files[0].fileName());
        totalFiles = currentFiles();
        totalSize = currentSize();
    }
}

void Cache::cache(QString const& fileName) {
    if(m_files.indexOf(fileName) == -1) {
        QFileInfo fileInfo(fileName);
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
    return m_files.indexOf(fileName) != -1 && QFile::exists(fileName);
}

void Cache::remove(QString const& fileName) {
    if(exists(fileName)) {
        QFile::remove(fileName);
        QFileInfo fileInfo(fileName);
        m_files.removeOne(fileInfo);
        emit currentFilesChanged(currentFiles());
        emit currentSizeChanged(currentSize());
        emit filesChanged();
    }
}

QDebug operator << (QDebug dbg, const Cache &c) {
    dbg.nospace()
        << "(Cache: currentFiles=" << c.currentFiles()
        << ", currentSize=" << c.currentSize()
        << ", maxFiles=" << c.maxFiles()
        << ", maxSize=" << c.maxSize()
        << ", files=" << c.files()
        << ", filesNameFilters=" << c.fileNameFilters()
        << ")";
    return dbg.space();
}
