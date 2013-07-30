#include "SongInfo.hpp"

SongInfo::SongInfo(QObject *parent)
 : SongBasicInfo(parent),
   m_patterns(0),
   m_orders(0),
   m_instruments(0),
   m_samples(0),
   m_channels(0) {
}

QString SongInfo::hash() const {
    return m_hash;
}

void SongInfo::setHash(const QString &value) {
    m_hash = value;
}

QString SongInfo::format() const {
    return m_format;
}

void SongInfo::setFormat(const QString &value) {
    m_format = value;
}

QString SongInfo::tracker() const {
    return m_tracker;
}

void SongInfo::setTracker(const QString &value) {
    m_tracker = value;
}

QString SongInfo::genre() const {
    return m_genre;
}

void SongInfo::setGenre(const QString &value) {
    m_genre = value;
}

QString SongInfo::artist() const {
    return m_artist;
}

void SongInfo::setArtist(const QString &value) {
    m_artist = value;
}

int SongInfo::patterns() const {
    return m_patterns;
}

void SongInfo::setPatterns(int value) {
    m_patterns = value;
}

int SongInfo::orders() const {
    return m_orders;
}

void SongInfo::setOrders(int value) {
    m_orders = value;
}

int SongInfo::instruments() const {
    return m_instruments;
}

void SongInfo::setInstruments(int value) {
    m_instruments = value;
}

int SongInfo::samples() const {
    return m_samples;
}

void SongInfo::setSamples(int value) {
    m_samples = value;
}

int SongInfo::channels() const {
    return m_channels;
}

void SongInfo::setChannels(int value) {
    m_channels = value;
}
