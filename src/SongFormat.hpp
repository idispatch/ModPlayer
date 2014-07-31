#ifndef SONGFORMAT_HPP_
#define SONGFORMAT_HPP_

#include <QUrl>
#include "ItemGroupBase.hpp"
#include "InstanceCounter.hpp"

class SongFormat : public ItemGroupBase,
                   public InstanceCounter<SongFormat> {
    Q_OBJECT
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString iconPath READ iconPath NOTIFY iconPathChanged FINAL)
public:
    enum Format {
        FORMAT_UNKNOWN = 0,
        FORMAT_MOD = 1,
        FORMAT_669 = 2,
        FORMAT_IT = 3,
        FORMAT_MED = 4,
        FORMAT_MTM = 5,
        FORMAT_OCT = 6,
        FORMAT_OKT = 7,
        FORMAT_S3M = 8,
        FORMAT_STM = 9,
        FORMAT_XM = 10,
        FORMAT_AHX = 11,
        FORMAT_HVL = 12,
        FORMAT_MO3 = 1,
        FORMAT_MP3 = 100,
        FORMAT_OGG = 101,
        FORMAT_FLAC = 102,
        FORMAT_HTTP = 1000
    };
    SongFormat(int id,
               QString const& name,
               QString const& description,
               int count,
               double duration,
               QObject *parent);
    ~SongFormat();
    QString const& description() const;
    QString iconPath() const;

    bool isTrackerSong() const;
    bool isMp3Song() const;
    bool isHttpSong() const;

    static QString getIconPath(Format formatId);
    static QString getIconPath(QString const& fileName);
    static Format getFormatIdByFileName(QString const& fileName);
    static QString getFormatByFormatId(Format formatId);

    static bool isTrackerSong(QString const& fileName);
    static bool isTrackerSong(Format formatId);

    static bool isMp3Song(QString const& fileName);
    static bool isMp3Song(Format formatId);

    static bool isHttpSong(QString const& fileName);
    static bool isHttpSong(Format formatId);

    using InstanceCounter<SongFormat>::getInstanceCount;
    using InstanceCounter<SongFormat>::getMaxInstanceCount;

Q_SIGNALS:
    void descriptionChanged();
    void iconPathChanged();
private:
    Q_DISABLE_COPY(SongFormat)
    QString m_description;
};

Q_DECLARE_METATYPE(SongFormat*);

QDebug operator << (QDebug dbg, SongFormat const &format);

#endif
