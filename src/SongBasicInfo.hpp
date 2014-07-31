#ifndef SONGBASICINFO_HPP_
#define SONGBASICINFO_HPP_

#include <QDebug>
#include <QMetaType>
#include <QObject>
#include <QVariant>
#include <QUrl>
#include "InstanceCounter.hpp"

class SongBasicInfo : public QObject,
                      public InstanceCounter<SongBasicInfo> {
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(bool isLocal READ isLocal NOTIFY isLocalChanged FINAL)
    Q_PROPERTY(bool isTrackerSong READ isTrackerSong NOTIFY isTrackerSongChanged FINAL)
    Q_PROPERTY(bool isHttpSong READ isHttpSong NOTIFY isHttpSongChanged FINAL)

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QUrl iconPath READ iconPath WRITE setIconPath NOTIFY iconPathChanged FINAL)

    Q_PROPERTY(int fileSize READ fileSize WRITE setFileSize NOTIFY fileSizeChanged FINAL)
    Q_PROPERTY(int songLength READ songLength WRITE setSongLength NOTIFY songLengthChanged FINAL)
    Q_PROPERTY(QString songLengthText READ songLengthText NOTIFY songLengthTextChanged FINAL)

    Q_PROPERTY(int formatId READ formatId NOTIFY formatIdChanged FINAL)
    Q_PROPERTY(int downloads READ downloads WRITE setDownloads NOTIFY downloadsChanged FINAL)
    Q_PROPERTY(int favourited READ favourited WRITE setFavourited NOTIFY favouritedChanged FINAL)
    Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged FINAL)

    Q_PROPERTY(int playCount READ playCount WRITE setPlayCount NOTIFY playCountChanged FINAL)
    Q_PROPERTY(int lastPlayed READ lastPlayed WRITE setLastPlayed NOTIFY lastPlayedChanged FINAL)
    Q_PROPERTY(int myFavourite READ myFavourite WRITE setMyFavourite NOTIFY myFavouriteChanged FINAL)

public:
    SongBasicInfo(QObject *parent);
    SongBasicInfo(int id,
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
                  QObject * parent);
    ~SongBasicInfo();
    SongBasicInfo& operator = (SongBasicInfo const& other);

    bool isTrackerSong() const;
    bool isHttpSong() const;

    QString fileName() const;
    void setFileName(const QString &value);

    QString title() const;
    void setTitle(const QString &value);

    int id() const;
    void setId(int value);

    bool isLocal() const;

    int fileSize() const;
    void setFileSize(int value);

    int songLength() const;
    QString songLengthText() const;
    void setSongLength(int value);

    int formatId() const;

    QUrl iconPath() const;
    void setIconPath(QUrl const& path);

    int downloads() const;
    void setDownloads(int value);

    int favourited() const;
    void setFavourited(int value);

    int score() const;
    void setScore(int value);

    int playCount() const;
    void setPlayCount(int value);

    int lastPlayed() const;
    void setLastPlayed(int value);

    int myFavourite() const;
    void setMyFavourite(int value);

    using InstanceCounter<SongBasicInfo>::getInstanceCount;
    using InstanceCounter<SongBasicInfo>::getMaxInstanceCount;
Q_SIGNALS:
    void isTrackerSongChanged();
    void isHttpSongChanged();
    void fileNameChanged();
    void titleChanged();
    void idChanged();
    void isLocalChanged();
    void formatIdChanged();
    void iconPathChanged();
    void fileSizeChanged();
    void songLengthChanged();
    void songLengthTextChanged();
    void downloadsChanged();
    void favouritedChanged();
    void scoreChanged();
    void playCountChanged();
    void lastPlayedChanged();
    void myFavouriteChanged();
private:
    int m_id;
    QString m_fileName;
    QString m_title;
    QUrl m_iconPath;
    int m_fileSize;
    int m_songLength;
    int m_downloads;
    int m_favourited;
    int m_score;
    int m_playCount;
    int m_lastPlayed;
    int m_myFavourite;
};

Q_DECLARE_METATYPE(SongBasicInfo*);

inline SongBasicInfo* songBasicInfo(QVariant const& value) {
    SongBasicInfo *result = qobject_cast<SongBasicInfo*>(value.value<QObject*>());
    if(result == 0) {
        qDebug() << "!!! Could not cast" << value << "to SongBasicInfo*";
    }
    return result;
}

#endif
