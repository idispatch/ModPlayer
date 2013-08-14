#include "Artist.hpp"
#include <QDebug>

Artist::Artist(int id,
               QString const& name,
               int score,
               int downloads,
               int rating,
               int count,
               QObject *parent)
    : ItemGroupBase(id, name, count, parent),
      m_score(score),
      m_downloads(downloads),
      m_rating(rating) {
}

Artist::~Artist() {
}

int Artist::score() const {
    return m_score;
}

int Artist::downloads() const {
    return m_downloads;
}

int Artist::rating() const {
    return m_rating;
}
