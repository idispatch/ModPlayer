#ifndef NAMEDPLAYLIST_HPP_
#define NAMEDPLAYLIST_HPP_

#include "ItemGroupBase.hpp"
#include "InstanceCounter.hpp"

class NamedPlaylist : public ItemGroupBase,
                      public InstanceCounter<NamedPlaylist> {
    Q_OBJECT
    Q_DISABLE_COPY(NamedPlaylist)
public:
    NamedPlaylist(int id,
                  QString const& name,
                  int count,
                  double duration,
                  QObject *parent);
    ~NamedPlaylist();

    using InstanceCounter<NamedPlaylist>::getInstanceCount;
    using InstanceCounter<NamedPlaylist>::getMaxInstanceCount;
};

Q_DECLARE_METATYPE(NamedPlaylist*);

#endif /* NAMEDPLAYLIST_HPP_ */
