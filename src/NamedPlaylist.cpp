#include "NamedPlaylist.hpp"
#include <QDebug>

template<>
int InstanceCounter<NamedPlaylist>::s_count;
template<>
int InstanceCounter<NamedPlaylist>::s_maxCount;

//#define VERBOSE_LOGGING

NamedPlaylist::NamedPlaylist(int id,
                             QString const& name,
                             int count,
                             int duration,
                             QObject *parent)
    : ItemGroupBase(id, name, count, duration, parent) {
}

NamedPlaylist::~NamedPlaylist() {
#ifdef VERBOSE_LOGGING
    qDebug() << "NamedPlaylist::~NamedPlaylist()";
#endif
}
