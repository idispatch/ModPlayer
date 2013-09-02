#ifndef SONGEXTENDEDINFO_HPP_
#define SONGEXTENDEDINFO_HPP_

#include <QDebug>
#include <QMetaType>
#include <QObject>
#include <QVariant>
#include "SongBasicInfo.hpp"
#include "InstanceCounter.hpp"

class SongExtendedInfo : public SongBasicInfo,
                         public InstanceCounter<SongExtendedInfo> {
    Q_OBJECT

    Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged FINAL)
    Q_PROPERTY(QString tracker READ tracker WRITE setTracker NOTIFY trackerChanged FINAL)
    Q_PROPERTY(QString genre READ genre WRITE setGenre NOTIFY genreChanged FINAL)
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged FINAL)
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged FINAL)

    Q_PROPERTY(int patterns READ patterns WRITE setPatterns NOTIFY patternsChanged FINAL)
    Q_PROPERTY(int orders READ orders WRITE setOrders NOTIFY ordersChanged FINAL)
    Q_PROPERTY(int instruments READ instruments WRITE setInstruments NOTIFY instrumentsChanged FINAL)
    Q_PROPERTY(int samples READ samples WRITE setSamples NOTIFY samplesChanged FINAL)
    Q_PROPERTY(int channels READ channels WRITE setChannels NOTIFY channelsChanged FINAL)

public:
    SongExtendedInfo(QObject *parent);
    SongExtendedInfo(int id,
                     QString const& fileName,
                     QString const& title,
                     int formatId,
                     int downloads,
                     int favourited,
                     int score,
                     int size,
                     int length,
                     int playCount,
                     int lastPlayed,
                     int myFavourite,
                     QString const& format,
                     QString const& tracker,
                     QString const& genre,
                     int artistId,
                     QString const& artist,
                     int patterns,
                     int orders,
                     int instruments,
                     int samples,
                     int channels,
                     QObject * parent);
    ~SongExtendedInfo();
    SongExtendedInfo& operator = (SongExtendedInfo const&);

    QString format() const;
    void setFormat(const QString &value);

    QString tracker() const;
    void setTracker(const QString &value);

    QString genre() const;
    void setGenre(const QString &value);

    int artistId() const;
    void setArtistId(int value);

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

    using InstanceCounter<SongExtendedInfo>::getInstanceCount;
    using InstanceCounter<SongExtendedInfo>::getMaxInstanceCount;
Q_SIGNALS:
    void formatChanged();
    void trackerChanged();
    void genreChanged();
    void artistIdChanged();
    void artistChanged();
    void patternsChanged();
    void ordersChanged();
    void instrumentsChanged();
    void samplesChanged();
    void channelsChanged();
private:
    QString m_format;
    QString m_tracker;
    QString m_genre;
    int m_artistId;
    QString m_artist;
    int m_patterns;
    int m_orders;
    int m_instruments;
    int m_samples;
    int m_channels;
};

Q_DECLARE_METATYPE(SongExtendedInfo*);

inline SongExtendedInfo* songExtendedInfo(QVariant const& value) {
    SongExtendedInfo *result = qobject_cast<SongExtendedInfo*>(value.value<QObject*>());
    if(result == 0) {
        qDebug() << "!!! Could not cast" << value << "to SongExtendedInfo*";
    }
    return result;
}

#endif
