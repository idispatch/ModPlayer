#include "SongBasicInfo.hpp"

SongBasicInfo::SongBasicInfo(QObject *parent)
 : QObject(parent),
   m_modId(0),
   m_fileSize(0),
   m_songLength(0),
   m_downloads(0),
   m_favourited(0),
   m_score(0) {
}

QString SongBasicInfo::fileName() const {
    return m_fileName;
}

void SongBasicInfo::setFileName(const QString &value) {
    m_fileName = value;
}

QString SongBasicInfo::title() const {
    return m_title;
}

void SongBasicInfo::setTitle(const QString &value) {
    m_title = value;
}

int SongBasicInfo::modId() const {
    return m_modId;
}

void SongBasicInfo::setModId(int value) {
    m_modId = value;
}

int SongBasicInfo::fileSize() const {
    return m_fileSize;
}

void SongBasicInfo::setFileSize(int value) {
    m_fileSize = value;
}

int SongBasicInfo::songLength() const {
    return m_songLength;
}

void SongBasicInfo::setSongLength(int value) {
    m_songLength = value;
}

int SongBasicInfo::downloads() const {
    return m_downloads;
}

void SongBasicInfo::setDownloads(int value) {
    m_downloads = value;
}

int SongBasicInfo::favourited() const {
    return m_favourited;
}

void SongBasicInfo::setFavourited(int value) {
    m_favourited = value;
}

int SongBasicInfo::score() const {
    return m_score;
}

void SongBasicInfo::setScore(int value) {
    m_score = value;
}
