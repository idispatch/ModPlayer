#include "SongGenre.hpp"
#include <QDebug>

template<>
int InstanceCounter<SongGenre>::s_count;
template<>
int InstanceCounter<SongGenre>::s_maxCount;

SongGenre::SongGenre(int id,
                     QString const& name,
                     int count,
                     int duration,
                     QObject *parent)
    : ItemGroupBase(id, name, count, duration, parent) {
}

SongGenre::~SongGenre() {
#ifdef VERBOSE_LOGGING
    qDebug() << "SongGenre::~SongGenre()";
#endif
}
