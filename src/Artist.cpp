#include "Artist.hpp"
#include <QDebug>

template<>
int InstanceCounter<Artist>::s_count;
template<>
int InstanceCounter<Artist>::s_maxCount;

Artist::Artist(int id,
               QString const& name,
               int score,
               int downloads,
               int rating,
               int count,
               double duration,
               QObject *parent)
    : ItemGroupBase(id, name, count, duration, parent),
      m_score(score),
      m_downloads(downloads),
      m_rating(rating) {
}

Artist::~Artist() {
#ifdef VERBOSE_LOGGING
    qDebug() << "Artist::~Artist()";
#endif
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
