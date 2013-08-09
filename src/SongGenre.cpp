#include "SongGenre.hpp"
#include <QDebug>

SongGenre::SongGenre(int id,
                     QString const& name,
                     int count,
                     QObject *parent)
    : QObject(parent),
      m_id(id),
      m_name(name),
      m_count(count) {
}

SongGenre::~SongGenre() {
    //qDebug() << "SongGenre::~SongGenre()";
}

int SongGenre::id() const {
    return m_id;
}

QString const& SongGenre::name() const {
    return m_name;
}

int SongGenre::count() const {
    return m_count;
}
