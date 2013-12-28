#ifndef ALBUM_HPP_
#define ALBUM_HPP_

#include "ItemGroupBase.hpp"
#include "InstanceCounter.hpp"

class Album : public ItemGroupBase,
              public InstanceCounter<Album> {
    Q_OBJECT
public:
    Album(int id,
          QString const& name,
          int count,
          QObject *parent);
    ~Album();

    using InstanceCounter<Album>::getInstanceCount;
    using InstanceCounter<Album>::getMaxInstanceCount;
private:
    Q_DISABLE_COPY(Album)
};

Q_DECLARE_METATYPE(Album*);

#endif
