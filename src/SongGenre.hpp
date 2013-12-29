#ifndef SONGGENRE_HPP_
#define SONGGENRE_HPP_

#include "ItemGroupBase.hpp"
#include "InstanceCounter.hpp"

class SongGenre : public ItemGroupBase,
                  public InstanceCounter<SongGenre> {
    Q_OBJECT
public:
    SongGenre(int id,
              QString const& name,
              int count,
              int duration,
              QObject *parent);
    ~SongGenre();

    using InstanceCounter<SongGenre>::getInstanceCount;
    using InstanceCounter<SongGenre>::getMaxInstanceCount;
Q_SIGNALS:
private:
    Q_DISABLE_COPY(SongGenre)
};

Q_DECLARE_METATYPE(SongGenre*);

#endif
