#include "SongFormat.hpp"
#include <QDebug>

SongFormat::SongFormat(int id,
                       QString const& name,
                       QString const& description,
                       int count,
                       QObject *parent)
    : ItemGroupBase(id, name, count, parent),
      m_description(description) {
}

SongFormat::~SongFormat() {
    //qDebug() << "SongFormat::~SongFormat()";
}

QString const& SongFormat::description() const {
    return m_description;
}

QDebug operator << (QDebug dbg, SongFormat const &format) {
    dbg.nospace() << "(SongFormat: id="
                  << format.id()
                  << ",name="
                  << format.name()
                  << ",description="
                  << format.description()
                  << ",count="
                  << format.count()
                  << ")";
    return dbg.space();
}
