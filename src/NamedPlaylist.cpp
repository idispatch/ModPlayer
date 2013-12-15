#include "NamedPlaylist.hpp"
#include <QDebug>

template<>
int InstanceCounter<NamedPlaylist>::s_count;
template<>
int InstanceCounter<NamedPlaylist>::s_maxCount;

//#define DETAILED_LOGGING

NamedPlaylist::NamedPlaylist(int id,
                             QString const& name,
                             int count,
                             QObject *parent)
    : ItemGroupBase(id, name, count, parent) {
}

NamedPlaylist::~NamedPlaylist() {
#ifdef DETAILED_LOGGING
    qDebug() << "NamedPlaylist::~NamedPlaylist()";
#endif
}
