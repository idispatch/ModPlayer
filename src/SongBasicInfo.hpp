#ifndef SONGBASICINFO_HPP_
#define SONGBASICINFO_HPP_

#include <QMetaType>
#include <QObject>

class SongBasicInfo : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(int modId READ modId WRITE setModId NOTIFY modIdChanged FINAL)
    Q_PROPERTY(int fileSize READ fileSize WRITE setFileSize NOTIFY fileSizeChanged FINAL)
    Q_PROPERTY(int songLength READ songLength WRITE setSongLength NOTIFY songLengthChanged FINAL)
    Q_PROPERTY(int formatId READ formatId WRITE setFormatId NOTIFY formatIdChanged FINAL)
    Q_PROPERTY(int downloads READ downloads WRITE setDownloads NOTIFY downloadsChanged FINAL)
    Q_PROPERTY(int favourited READ favourited WRITE setFavourited NOTIFY favouritedChanged FINAL)
    Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged FINAL)
    Q_PROPERTY(int playCount READ playCount WRITE setPlayCount NOTIFY playCountChanged FINAL)
    Q_PROPERTY(int lastPlayed READ lastPlayed WRITE setLastPlayed NOTIFY lastPlayedChanged FINAL)
    Q_PROPERTY(bool myFavourite READ myFavourite WRITE setMyFavourite NOTIFY myFavouriteChanged FINAL)

public:
    SongBasicInfo(QObject *parent = 0);
    SongBasicInfo(int id,
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
                  QObject * parent = 0);
    ~SongBasicInfo();

    QString fileName() const;
    void setFileName(const QString &value);

    QString title() const;
    void setTitle(const QString &value);

    int modId() const;
    void setModId(int value);

    int fileSize() const;
    void setFileSize(int value);

    int songLength() const;
    void setSongLength(int value);

    int formatId() const;
    void setFormatId(int value);

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

    bool myFavourite() const;
    void setMyFavourite(bool value);
Q_SIGNALS:
    void fileNameChanged();
    void titleChanged();
    void modIdChanged();
    void formatIdChanged();
    void fileSizeChanged();
    void songLengthChanged();
    void downloadsChanged();
    void favouritedChanged();
    void scoreChanged();
    void playCountChanged();
    void lastPlayedChanged();
    void myFavouriteChanged();
private:
    Q_DISABLE_COPY(SongBasicInfo)

    QString m_fileName;
    QString m_title;
    int m_modId;
    int m_formatId;
    int m_fileSize;
    int m_songLength;
    int m_downloads;
    int m_favourited;
    int m_score;
    int m_playCount;
    int m_lastPlayed;
    bool m_myFavourite;
};

Q_DECLARE_METATYPE(SongBasicInfo*);

#endif
