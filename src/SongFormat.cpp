#include "SongFormat.hpp"
#include <QDebug>

SongFormat::SongFormat(int id,
                       QString const& name,
                       QString const& description,
                       int count,
                       QObject *parent)
    : QObject(parent),
      m_id(id),
      m_name(name),
      m_description(description),
      m_count(count) {
}

SongFormat::~SongFormat() {
    //qDebug() << "SongFormat::~SongFormat()";
}

int SongFormat::id() const {
    return m_id;
}

QString const& SongFormat::name() const {
    return m_name;
}

QString const& SongFormat::description() const {
    return m_description;
}

int SongFormat::count() const {
    return m_count;
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
