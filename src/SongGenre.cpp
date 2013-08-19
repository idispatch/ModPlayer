#include "SongGenre.hpp"
#include <QDebug>

template<>
int InstanceCounter<SongGenre>::s_count;
template<>
int InstanceCounter<SongGenre>::s_maxCount;

SongGenre::SongGenre(int id,
                     QString const& name,
                     int count,
                     QObject *parent)
    : ItemGroupBase(id, name, count, parent) {
}

SongGenre::~SongGenre() {
}
