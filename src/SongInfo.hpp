#ifndef SONGINFO_HPP_
#define SONGINFO_HPP_

#include <QDebug>
#include <QMetaType>
#include <QObject>
#include "SongBasicInfo.hpp"

class SongInfo : public SongBasicInfo {
    Q_OBJECT

    Q_PROPERTY(QString hash READ hash WRITE setHash FINAL)
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

    QString hash() const;
    void setHash(const QString &value);

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
private:
    Q_DISABLE_COPY(SongInfo)

    QString m_hash;
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
