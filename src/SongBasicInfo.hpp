#ifndef SONGBASICINFO_HPP_
#define SONGBASICINFO_HPP_

#include <QMetaType>
#include <QObject>

class SongBasicInfo : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle FINAL)
    Q_PROPERTY(int modId READ modId WRITE setModId FINAL)
    Q_PROPERTY(int fileSize READ fileSize WRITE setFileSize FINAL)
    Q_PROPERTY(int songLength READ songLength WRITE setSongLength FINAL)
    Q_PROPERTY(int downloads READ downloads WRITE setDownloads FINAL)
    Q_PROPERTY(int favourited READ favourited WRITE setFavourited FINAL)
    Q_PROPERTY(int score READ score WRITE setScore FINAL)

public:
    SongBasicInfo(QObject *parent = 0);

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

    int downloads() const;
    void setDownloads(int value);

    int favourited() const;
    void setFavourited(int value);

    int score() const;
    void setScore(int value);

private:
    Q_DISABLE_COPY(SongBasicInfo)

    QString m_fileName;
    QString m_title;
    int m_modId;
    int m_fileSize;
    int m_songLength;
    int m_downloads;
    int m_favourited;
    int m_score;
};

Q_DECLARE_METATYPE(SongBasicInfo*);

#endif
