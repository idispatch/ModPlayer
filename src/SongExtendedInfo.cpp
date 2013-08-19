#include "SongExtendedInfo.hpp"

template<>
int InstanceCounter<SongExtendedInfo>::s_count;
template<>
int InstanceCounter<SongExtendedInfo>::s_maxCount;

SongExtendedInfo::SongExtendedInfo(QObject *parent)
 : SongBasicInfo(parent),
   m_patterns(0),
   m_orders(0),
   m_instruments(0),
   m_samples(0),
   m_channels(0) {
}

SongExtendedInfo::SongExtendedInfo(int id,
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
                                   QString const& artist,
                                   int patterns,
                                   int orders,
                                   int instruments,
                                   int samples,
                                   int channels,
                                   QObject * parent)
    : SongBasicInfo(id,
                    fileName,
                    title,
                    formatId,
                    downloads,
                    favourited,
                    score,
                    size,
                    length,
                    playCount,
                    lastPlayed,
                    myFavourite,
                    parent),
      m_format(format),
      m_tracker(tracker),
      m_genre(genre),
      m_artist(artist),
      m_patterns(patterns),
      m_orders(orders),
      m_instruments(instruments),
      m_samples(samples),
      m_channels(channels) {
}

SongExtendedInfo::~SongExtendedInfo() {
}

SongExtendedInfo& SongExtendedInfo::operator = (SongExtendedInfo const& other) {
    if(this != &other) {
        SongBasicInfo::operator = (other);
        setFormat(other.format());
        setTracker(other.tracker());
        setGenre(other.genre());
        setArtist(other.artist());
        setPatterns(other.patterns());
        setOrders(other.orders());
        setInstruments(other.instruments());
        setSamples(other.samples());
        setChannels(other.channels());
    }
    return *this;
}

QString SongExtendedInfo::format() const {
    return m_format;
}

void SongExtendedInfo::setFormat(const QString &value) {
    if(m_format != value) {
        m_format = value;
        emit formatChanged();
    }
}

QString SongExtendedInfo::tracker() const {
    return m_tracker;
}

void SongExtendedInfo::setTracker(const QString &value) {
    if(m_tracker != value) {
        m_tracker = value;
        emit trackerChanged();
    }
}

QString SongExtendedInfo::genre() const {
    return m_genre;
}

void SongExtendedInfo::setGenre(const QString &value) {
    if(m_genre != value) {
        m_genre = value;
        emit genreChanged();
    }
}

QString SongExtendedInfo::artist() const {
    return m_artist;
}

void SongExtendedInfo::setArtist(const QString &value) {
    if(m_artist != value) {
        m_artist = value;
        emit artistChanged();
    }
}

int SongExtendedInfo::patterns() const {
    return m_patterns;
}

void SongExtendedInfo::setPatterns(int value) {
    if(m_patterns != value) {
        m_patterns = value;
        emit patternsChanged();
    }
}

int SongExtendedInfo::orders() const {
    return m_orders;
}

void SongExtendedInfo::setOrders(int value) {
    if(m_orders != value) {
        m_orders = value;
        emit ordersChanged();
    }
}

int SongExtendedInfo::instruments() const {
    return m_instruments;
}

void SongExtendedInfo::setInstruments(int value) {
    if(m_instruments != value) {
        m_instruments = value;
        emit instrumentsChanged();
    }
}

int SongExtendedInfo::samples() const {
    return m_samples;
}

void SongExtendedInfo::setSamples(int value) {
    if(m_samples != value) {
        m_samples = value;
        emit samplesChanged();
    }
}

int SongExtendedInfo::channels() const {
    return m_channels;
}

void SongExtendedInfo::setChannels(int value) {
    if(m_channels != value) {
        m_channels = value;
        emit channelsChanged();
    }
}
