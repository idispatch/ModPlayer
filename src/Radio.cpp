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
             int bitrate,
             QObject *parent)
    : ItemGroupBase(id, name, 0, 0, parent),
      m_playlist(radioPlaylist),
      m_country(country),
      m_location(location),
      m_style(style),
      m_url(url),
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

int Radio::bitrate() const {
    return m_bitrate;
}

