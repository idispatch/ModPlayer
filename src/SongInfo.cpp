#include "SongInfo.hpp"

SongInfo::SongInfo(QObject *parent)
 : QObject(parent),
   m_modId(0),
   m_fileSize(0),
   m_songLength(0),
   m_downloads(0),
   m_favourited(0),
   m_score(0),
   m_patterns(0),
   m_orders(0),
   m_instruments(0),
   m_samples(0),
   m_channels(0) {
}

QString SongInfo::fileName() const {
    return m_fileName;
}

void SongInfo::setFileName(const QString &value) {
    m_fileName = value;
}

QString SongInfo::title() const {
    return m_title;
}

void SongInfo::setTitle(const QString &value) {
    m_title = value;
}

QString SongInfo::hash() const {
    return m_hash;
}

void SongInfo::setHash(const QString &value) {
    m_hash = value;
}

int SongInfo::modId() const {
    return m_modId;
}

void SongInfo::setModId(int value) {
    m_modId = value;
}

int SongInfo::fileSize() const {
    return m_fileSize;
}

void SongInfo::setFileSize(int value) {
    m_fileSize = value;
}

int SongInfo::songLength() const {
    return m_songLength;
}

void SongInfo::setSongLength(int value) {
    m_songLength = value;
}

int SongInfo::downloads() const {
    return m_downloads;
}

void SongInfo::setDownloads(int value) {
    m_downloads = value;
}

int SongInfo::favourited() const {
    return m_favourited;
}

void SongInfo::setFavourited(int value) {
    m_favourited = value;
}

int SongInfo::score() const {
    return m_score;
}

void SongInfo::setScore(int value) {
    m_score = value;
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
