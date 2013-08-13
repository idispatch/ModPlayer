#include "SongGenre.hpp"
#include <QDebug>

SongGenre::SongGenre(int id,
                     QString const& name,
                     int count,
                     QObject *parent)
    : ItemGroupBase(id, name, count, parent) {
}

SongGenre::~SongGenre() {
    //qDebug() << "SongGenre::~SongGenre()";
}
