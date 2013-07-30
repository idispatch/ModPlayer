#ifndef SONGINFO_HPP_
#define SONGINFO_HPP_

#include <QMetaType>
#include <QObject>

class SongInfo : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle FINAL)
    Q_PROPERTY(QString hash READ hash WRITE setHash FINAL)
    Q_PROPERTY(int modId READ modId WRITE setModId FINAL)
    Q_PROPERTY(int fileSize READ fileSize WRITE setFileSize FINAL)
    Q_PROPERTY(int songLength READ songLength WRITE setSongLength FINAL)
    Q_PROPERTY(int downloads READ downloads WRITE setDownloads FINAL)
    Q_PROPERTY(int favourited READ favourited WRITE setFavourited FINAL)
    Q_PROPERTY(int score READ score WRITE setScore FINAL)
    Q_PROPERTY(QString format READ format WRITE setFormat FINAL)
    Q_PROPERTY(QString tracker READ tracker WRITE setTracker FINAL)
    Q_PROPERTY(QString genre READ genre WRITE setGenre FINAL)
    Q_PROPERTY(QString artist READ artist WRITE setArtist FINAL)
    Q_PROPERTY(int patterns READ patterns WRITE setPatterns FINAL)
    Q_PROPERTY(int orders READ orders WRITE setOrders FINAL)
    Q_PROPERTY(int instruments READ instruments WRITE setInstruments FINAL)
    Q_PROPERTY(int samples READ samples WRITE setSamples FINAL)
    Q_PROPERTY(int channels READ channels WRITE setChannels FINAL)

public:
    SongInfo(QObject *parent = 0);

    QString fileName() const;
    void setFileName(const QString &value);

    QString title() const;
    void setTitle(const QString &value);

    QString hash() const;
    void setHash(const QString &value);

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

    QString format() const;
    void setFormat(const QString &value);

    QString tracker() const;
    void setTracker(const QString &value);

    QString genre() const;
    void setGenre(const QString &value);

    QString artist() const;
    void setArtist(const QString &value);

    int patterns() const;
    void setPatterns(int value);

    int orders() const;
    void setOrders(int value);

    int instruments() const;
    void setInstruments(int value);

    int samples() const;
    void setSamples(int value);

    int channels() const;
    void setChannels(int value);


Q_SIGNALS:
private:
    Q_DISABLE_COPY(SongInfo)

    QString m_fileName;
    QString m_title;
    QString m_hash;
    int m_modId;
    int m_fileSize;
    int m_songLength;
    int m_downloads;
    int m_favourited;
    int m_score;
    QString m_format;
    QString m_tracker;
    QString m_genre;
    QString m_artist;
    int m_patterns;
    int m_orders;
    int m_instruments;
    int m_samples;
    int m_channels;
};

Q_DECLARE_METATYPE(SongInfo*);

#endif
