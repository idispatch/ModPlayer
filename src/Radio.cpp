#include "Radio.hpp"
#include <QDebug>

template<>
int InstanceCounter<Radio>::s_count;
template<>
int InstanceCounter<Radio>::s_maxCount;

//#define VERBOSE_LOGGING

Radio::Radio(int id,
             QString const& name,
             QString const& radioPlaylist,
             QString const& country,
             QString const& location,
             QString const& style,
             QString const& url,
             QString const& flag,
             int bitrate,
             QObject *parent)
    : NamedItem(id, name, parent),
      m_playlist(radioPlaylist),
      m_country(country),
      m_location(location),
      m_style(style),
      m_url(url),
      m_flag(flag),
      m_bitrate(bitrate) {
}

Radio::~Radio() {
#ifdef VERBOSE_LOGGING
    qDebug() << "Radio::~Radio()";
#endif
}

QString const& Radio::radioPlaylist() const {
    return m_playlist;
}

QString const& Radio::country() const {
    return m_country;
}

QString const& Radio::location() const {
    return m_location;
}

QString const& Radio::style() const {
    return m_style;
}

QString const& Radio::url() const {
    return m_url;
}

QString const& Radio::flag() const {
    return m_flag;
}

int Radio::bitrate() const {
    return m_bitrate;
}

QDebug operator << (QDebug dbg, Radio const &p) {
    dbg.nospace() << "id: " << p.id()
                  << ", name: " << p.name()
                  << ", playlist: " << p.radioPlaylist()
                  << ", style: " << p.style()
                  << ", country: " << p.country()
                  << ", location: " << p.location();
    return dbg.space();
}
