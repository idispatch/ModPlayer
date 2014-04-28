#include "SongBasicInfo.hpp"
#include "SongFormat.hpp"
#include "FileUtils.hpp"
#include <QDebug>

template<>
int InstanceCounter<SongBasicInfo>::s_count;
template<>
int InstanceCounter<SongBasicInfo>::s_maxCount;

SongBasicInfo::SongBasicInfo(QObject *parent)
 : QObject(parent),
   m_id(0),
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
                             int downloads,
                             int favourited,
                             int score,
                             int size,
                             int length,
                             int playCount,
                             int lastPlayed,
                             int myFavourite,
                             QObject * parent)
   : QObject(parent),
     m_id(id),
     m_fileName(fileName),
     m_title(title),
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
        m_title = FileUtils::fileNameOnly(m_fileName);
    }
}

SongBasicInfo::~SongBasicInfo() {
#ifdef VERBOSE_LOGGING
    qDebug() << "SongBasicInfo::~SongBasicInfo()";
#endif
}

SongBasicInfo& SongBasicInfo::operator = (SongBasicInfo const& other) {
    if(this != & other) {
        setId(other.id());
        setFileName(other.fileName());
        setTitle(other.title());
        setFileSize(other.fileSize());
        setSongLength(other.songLength());
        setDownloads(other.downloads());
        setFavourited(other.favourited());
        setScore(other.score());
        setPlayCount(other.playCount());
        setLastPlayed(other.lastPlayed());
        setMyFavourite(other.myFavourite());
    }
    return *this;
}

QString SongBasicInfo::fileName() const {
    return m_fileName;
}

void SongBasicInfo::setFileName(const QString &value) {
    if(m_fileName != value) {
        int oldFormatId = formatId();
        bool oldIsTrackerSong = isTrackerSong();
        bool oldIsHttpSong = isHttpSong();
        QUrl oldIconPath = iconPath();

        m_fileName = value;

        emit fileNameChanged();
        if(oldIconPath != iconPath()) {
            emit iconPathChanged();
        }
        if(oldIsHttpSong != isHttpSong()) {
            emit isHttpSongChanged();
        }
        if(oldIsTrackerSong != isTrackerSong()) {
            emit isTrackerSongChanged();
        }
        if(oldFormatId != formatId()) {
            emit formatIdChanged();
        }
    }
}

QString SongBasicInfo::title() const {
    return m_title;
}

void SongBasicInfo::setTitle(const QString &value) {
    QString newTitle(value);
    newTitle = newTitle.trimmed();
    if(newTitle.length() == 0) {
        newTitle = FileUtils::fileNameOnly(m_fileName);
    }
    if(m_title != newTitle) {
        m_title = newTitle;
        emit titleChanged();
    }
}

int SongBasicInfo::id() const {
    return m_id;
}

void SongBasicInfo::setId(int value) {
    if(m_id != value) {
        bool oldIsLocal = isLocal();
        m_id = value;
        emit idChanged();
        if(oldIsLocal != isLocal()) {
            emit isLocalChanged();
        }
    }
}

bool SongBasicInfo::isLocal() const {
    return m_id < 0;
}

int SongBasicInfo::formatId() const {
    return SongFormat::getFormatIdByFileName(m_fileName);
}

QUrl SongBasicInfo::iconPath() const {
    return SongFormat::getIconPath(static_cast<SongFormat::Format>(formatId()));
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

bool SongBasicInfo::isTrackerSong() const {
    return SongFormat::isTrackerSong(static_cast<SongFormat::Format>(formatId()));
}

bool SongBasicInfo::isHttpSong() const {
    return SongFormat::isHttpSong(static_cast<SongFormat::Format>(formatId()));
}

int SongBasicInfo::songLength() const {
    return m_songLength;
}

QString SongBasicInfo::songLengthText() const {
    int totalSeconds = songLength() / 1000;
    int seconds = totalSeconds % 60;
    int minutes = totalSeconds / 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}

void SongBasicInfo::setSongLength(int value) {
    if(m_songLength != value) {
        m_songLength = value;
        emit songLengthChanged();
        emit songLengthTextChanged();
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

int SongBasicInfo::myFavourite() const {
    return m_myFavourite;
}

void SongBasicInfo::setMyFavourite(int value) {
    if(m_myFavourite != value) {
        m_myFavourite = value;
        emit myFavouriteChanged();
    }
}
