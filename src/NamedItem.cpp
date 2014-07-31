#include "NamedItem.hpp"
#include <QDebug>

template<>
int InstanceCounter<NamedItem>::s_count;
template<>
int InstanceCounter<NamedItem>::s_maxCount;

NamedItem::NamedItem(int id,
                     QString const& name,
                     QObject *parent)
    : QObject(parent),
      m_id(id),
      m_name(name) {
}

NamedItem::~NamedItem() {
}

int NamedItem::id() const {
    return m_id;
}

QString const& NamedItem::name() const {
    return m_name;
}

QDebug operator << (QDebug dbg, NamedItem const &item) {
    dbg.nospace() << "(NamedItem: id="
                  << item.id()
                  << ",name="
                  << item.name()
                  << ")";
    return dbg.space();
}
