#ifndef NAMEDPLAYLIST_HPP_
#define NAMEDPLAYLIST_HPP_

#include "ItemGroupBase.hpp"
#include "InstanceCounter.hpp"

class NamedPlaylist : public ItemGroupBase,
                      public InstanceCounter<NamedPlaylist> {
    Q_OBJECT
public:
    NamedPlaylist(int id,
                  QString const& name,
                  int count,
                  double duration,
                  QObject *parent);
    ~NamedPlaylist();

    using InstanceCounter<NamedPlaylist>::getInstanceCount;
    using InstanceCounter<NamedPlaylist>::getMaxInstanceCount;
private:
    Q_DISABLE_COPY(NamedPlaylist)
};

Q_DECLARE_METATYPE(NamedPlaylist*);

#endif /* NAMEDPLAYLIST_HPP_ */
