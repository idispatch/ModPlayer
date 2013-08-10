#include "SongBasicInfo.hpp"
#include <QDebug>

SongBasicInfo::SongBasicInfo(QObject *parent)
 : QObject(parent),
   m_modId(0),
   m_formatId(0),
   m_fileSize(0),
   m_songLength(0),
   m_downloads(0),
   m_favourited(0),
   m_score(0),
   m_playCount(0),
   m_lastPlayed(0),
   m_myFavourite(false) {
}

SongBasicInfo::SongBasicInfo(int id,
                             QString const& fileName,
                             QString const& title,
                             int format,
                             int downloads,
                             int favourited,
                             int score,
                             int size,
                             int length,
                             int playCount,
                             int lastPlayed,
                             bool myFavourite,
                             QObject * parent)
   : QObject(parent),
     m_fileName(fileName),
     m_title(title),
     m_modId(id),
     m_formatId(format),
     m_fileSize(size),
     m_songLength(length),
     m_downloads(downloads),
     m_favourited(favourited),
     m_score(score),
     m_playCount(playCount),
     m_lastPlayed(lastPlayed),
     m_myFavourite(myFavourite) {
    m_title = m_title.trimmed();
    if(m_title.length() == 0) {
        m_title = m_fileName;
    }
}

SongBasicInfo::~SongBasicInfo() {
    //qDebug() << "SongBasicInfo::~SongBasicInfo()";
}

QString SongBasicInfo::fileName() const {
    return m_fileName;
}

void SongBasicInfo::setFileName(const QString &value) {
    if(m_fileName != value) {
        m_fileName = value;
        emit fileNameChanged();
    }
}

QString SongBasicInfo::title() const {
    return m_title;
}

void SongBasicInfo::setTitle(const QString &value) {
    if(m_title != value) {
        QString newTitle(value);
        newTitle = newTitle.trimmed();
        if(newTitle.length() == 0) {
            newTitle = m_fileName;
        }
        if(newTitle != m_title) {
            m_title = newTitle;
            emit titleChanged();
        }
    }
}

int SongBasicInfo::modId() const {
    return m_modId;
}

void SongBasicInfo::setModId(int value) {
    if(m_modId != value) {
        m_modId = value;
        emit modIdChanged();
    }
}

int SongBasicInfo::formatId() const {
    return m_formatId;
}

void SongBasicInfo::setFormatId(int value) {
    if(m_formatId != value) {
        m_formatId = value;
        emit formatIdChanged();
    }
}

int SongBasicInfo::fileSize() const {
    return m_fileSize;
}

void SongBasicInfo::setFileSize(int value) {
    if(m_fileSize != value) {
        m_fileSize = value;
        emit fileSizeChanged();
    }
}

int SongBasicInfo::songLength() const {
    return m_songLength;
}

void SongBasicInfo::setSongLength(int value) {
    if(m_songLength != value) {
        m_songLength = value;
        emit songLengthChanged();
    }
}

int SongBasicInfo::downloads() const {
    return m_downloads;
}

void SongBasicInfo::setDownloads(int value) {
    if(m_downloads != value) {
        m_downloads = value;
        emit downloadsChanged();
    }
}

int SongBasicInfo::favourited() const {
    return m_favourited;
}

void SongBasicInfo::setFavourited(int value) {
    if(m_favourited != value) {
        m_favourited = value;
        emit favouritedChanged();
    }
}

int SongBasicInfo::score() const {
    return m_score;
}

void SongBasicInfo::setScore(int value) {
    if(m_score != value) {
        m_score = value;
        emit scoreChanged();
    }
}

int SongBasicInfo::playCount() const {
    return m_playCount;
}

void SongBasicInfo::setPlayCount(int value) {
    if(m_playCount != value) {
        m_playCount = value;
        emit playCountChanged();
    }
}

int SongBasicInfo::lastPlayed() const {
    return m_lastPlayed;
}

void SongBasicInfo::setLastPlayed(int value) {
    if(m_lastPlayed != value) {
        m_lastPlayed = value;
        emit lastPlayedChanged();
    }
}

bool SongBasicInfo::myFavourite() const {
    return m_myFavourite;
}

void SongBasicInfo::setMyFavourite(bool value) {
    if(m_myFavourite != value) {
        m_myFavourite = value;
        emit myFavouriteChanged();
    }
}
