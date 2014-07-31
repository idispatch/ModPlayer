#include "ItemGroupBase.hpp"
#include <QDebug>

template<>
int InstanceCounter<ItemGroupBase>::s_count;
template<>
int InstanceCounter<ItemGroupBase>::s_maxCount;

ItemGroupBase::ItemGroupBase(int id,
                             QString const& name,
                             int count,
                             double duration,
                             QObject *parent)
    : NamedItem(id, name, parent),
      m_count(count),
      m_duration(duration){
}

ItemGroupBase::~ItemGroupBase() {
}

int ItemGroupBase::count() const {
    return m_count;
}

double ItemGroupBase::duration() const {
    return m_duration;
}

QDebug operator << (QDebug dbg, ItemGroupBase const &group) {
    dbg.nospace() << "(ItemGroupBase: id="
                  << group.id()
                  << ",name="
                  << group.name()
                  << ",count="
                  << group.count()
                  << ",duration="
                  << group.duration()
                  << ")";
    return dbg.space();
}
