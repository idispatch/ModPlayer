#include "Album.hpp"
#include <QDebug>

template<>
int InstanceCounter<Album>::s_count;
template<>
int InstanceCounter<Album>::s_maxCount;

//#define DETAILED_LOGGING

Album::Album(int id,
             QString const& artistName,
             QString const& name,
             int count,
             QObject *parent)
    : ItemGroupBase(id, name, count, parent),
      m_artistName(artistName) {
}

Album::~Album() {
#ifdef DETAILED_LOGGING
    qDebug() << "Album::~Album()";
#endif
}

QString const& Album::artistName() const {
    return m_artistName;
}
