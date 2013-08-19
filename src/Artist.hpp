#ifndef ARTIST_HPP_
#define ARTIST_HPP_

#include "ItemGroupBase.hpp"
#include "InstanceCounter.hpp"

class Artist : public ItemGroupBase,
               public InstanceCounter<Artist> {
    Q_OBJECT
    Q_PROPERTY(int score READ score NOTIFY scoreChanged FINAL)
    Q_PROPERTY(int downloads READ downloads NOTIFY downloadsChanged FINAL)
    Q_PROPERTY(int rating READ rating NOTIFY ratingChanged FINAL)
public:
    Artist(int id,
           QString const& name,
           int score,
           int downloads,
           int rating,
           int count,
           QObject *parent = 0);
    ~Artist();

    int score() const;
    int downloads() const;
    int rating() const;

    using InstanceCounter<Artist>::getInstanceCount;
    using InstanceCounter<Artist>::getMaxInstanceCount;
Q_SIGNALS:
    void scoreChanged();
    void downloadsChanged();
    void ratingChanged();
private:
    Q_DISABLE_COPY(Artist)
    int m_score;
    int m_downloads;
    int m_rating;
};

Q_DECLARE_METATYPE(Artist*);

#endif
