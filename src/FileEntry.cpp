#include "FileEntry.hpp"
#include "FileUtils.hpp"
#include "SongFormat.hpp"
#include <sys/stat.h>

template<>
int InstanceCounter<FileEntry>::s_count;
template<>
int InstanceCounter<FileEntry>::s_maxCount;

FileEntry::FileEntry(QString const & path, struct stat64 const& stat, QObject * parent)
    : QObject(parent),
      m_stat(stat),
      m_path(path) {
}

bool FileEntry::isDirectory() const {
    return m_stat.st_mode & S_IFDIR;
}

bool FileEntry::isFile() const {
    return m_stat.st_mode & S_IFREG;
}

QString FileEntry::fileIcon() const {
    if(isFile()) {
        return SongFormat::getIconPath(m_path);
    } else {
        return "asset:///images/formats/icon_directory.png";
    }
}

QString FileEntry::fileName() const {
    return FileUtils::fileNameOnly(m_path);
}

QString FileEntry::filePath() const {
    return m_path;
}

QString FileEntry::fileType() const {
    if(isDirectory()) {
        return tr("Directory");
    } else {
        return SongFormat::getFormatByFileName(m_path);
    }
}

int FileEntry::fileSize() const {
    return m_stat.st_size;
}

