#include "ItemGroupBase.hpp"
#include <QDebug>

template<>
int InstanceCounter<ItemGroupBase>::s_count;
template<>
int InstanceCounter<ItemGroupBase>::s_maxCount;

ItemGroupBase::ItemGroupBase(int id,
                             QString const& name,
                             int count,
                             QObject *parent)
    : QObject(parent),
      m_id(id),
      m_name(name),
      m_count(count) {
}

ItemGroupBase::~ItemGroupBase() {
}

int ItemGroupBase::id() const {
    return m_id;
}

QString const& ItemGroupBase::name() const {
    return m_name;
}

int ItemGroupBase::count() const {
    return m_count;
}

QDebug operator << (QDebug dbg, ItemGroupBase const &group) {
    dbg.nospace() << "(ItemGroupBase: id="
                  << group.id()
                  << ",name="
                  << group.name()
                  << ",count="
                  << group.count()
                  << ")";
    return dbg.space();
}
