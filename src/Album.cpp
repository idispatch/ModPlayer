#include "Album.hpp"
#include <QDebug>

template<>
int InstanceCounter<Album>::s_count;
template<>
int InstanceCounter<Album>::s_maxCount;

//#define VERBOSE_LOGGING

Album::Album(int id,
             QString const& artistName,
             QString const& name,
             int count,
             int duration,
             QObject *parent)
    : ItemGroupBase(id, name, count, duration, parent),
      m_artistName(artistName) {
}

Album::~Album() {
#ifdef VERBOSE_LOGGING
    qDebug() << "Album::~Album()";
#endif
}

QString const& Album::artistName() const {
    return m_artistName;
}
