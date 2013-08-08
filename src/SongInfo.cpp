#include "SongInfo.hpp"

SongInfo::SongInfo(QObject *parent)
 : SongBasicInfo(parent),
   m_patterns(0),
   m_orders(0),
   m_instruments(0),
   m_samples(0),
   m_channels(0) {
}

SongInfo::SongInfo(int id,
                   QString const& fileName,
                   QString const& title,
                   int downloads,
                   int favourited,
                   int score,
                   int size,
                   int length,
                   int playCount,
                   int lastPlayed,
                   bool myFavourite,
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

SongInfo::~SongInfo() {
    qDebug() << "SongInfo::~SongInfo()";
}

QString SongInfo::format() const {
    return m_format;
}

void SongInfo::setFormat(const QString &value) {
    if(m_format != value) {
        m_format = value;
        emit formatChanged();
    }
}

QString SongInfo::tracker() const {
    return m_tracker;
}

void SongInfo::setTracker(const QString &value) {
    if(m_tracker != value) {
        m_tracker = value;
        emit trackerChanged();
    }
}

QString SongInfo::genre() const {
    return m_genre;
}

void SongInfo::setGenre(const QString &value) {
    if(m_genre != value) {
        m_genre = value;
        emit genreChanged();
    }
}

QString SongInfo::artist() const {
    return m_artist;
}

void SongInfo::setArtist(const QString &value) {
    if(m_artist != value) {
        m_artist = value;
        emit artistChanged();
    }
}

int SongInfo::patterns() const {
    return m_patterns;
}

void SongInfo::setPatterns(int value) {
    if(m_patterns != value) {
        m_patterns = value;
        emit patternsChanged();
    }
}

int SongInfo::orders() const {
    return m_orders;
}

void SongInfo::setOrders(int value) {
    if(m_orders != value) {
        m_orders = value;
        emit ordersChanged();
    }
}

int SongInfo::instruments() const {
    return m_instruments;
}

void SongInfo::setInstruments(int value) {
    if(m_instruments != value) {
        m_instruments = value;
        emit instrumentsChanged();
    }
}

int SongInfo::samples() const {
    return m_samples;
}

void SongInfo::setSamples(int value) {
    if(m_samples != value) {
        m_samples = value;
        emit samplesChanged();
    }
}

int SongInfo::channels() const {
    return m_channels;
}

void SongInfo::setChannels(int value) {
    if(m_channels != value) {
        m_channels = value;
        emit channelsChanged();
    }
}
