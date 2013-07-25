#include "SongInfo.hpp"

SongInfo::SongInfo(QObject *parent)
 : QObject(parent),
   m_modId(0) {
}

SongInfo::SongInfo(const QString &fileName,
                   const QString &title,
                   const QString &hash,
                   int modId,
                   QObject *parent)
    : QObject(parent),
      m_fileName(fileName),
      m_title(title),
      m_hash(hash),
      m_modId(modId)
{
}

SongInfo::SongInfo(const SongInfo& other)
    : QObject(other.parent()) {
    m_fileName = other.m_fileName;
    m_title = other.m_title;
    m_hash = other.m_hash;
    m_modId = other.m_modId;
}

QString SongInfo::fileName() const {
    return m_fileName;
}

QString SongInfo::title() const {
    return m_title;
}

QString SongInfo::hash() const {
    return m_hash;
}

int SongInfo::modId() const {
    return m_modId;
}

void SongInfo::setFileName(const QString &value) {
    m_fileName = value;
}

void SongInfo::setTitle(const QString &value) {
    m_title = value;
}

void SongInfo::setHash(const QString &value) {
    m_hash = value;
}

void SongInfo::setModId(int value) {
    m_modId = value;
}

