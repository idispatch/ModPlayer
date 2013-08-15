#include "SongFormat.hpp"
#include <QDebug>
#include <QDir>

SongFormat::SongFormat(int id,
                       QString const& name,
                       QString const& description,
                       int count,
                       QObject *parent)
    : ItemGroupBase(id, name, count, parent),
      m_description(description) {
}

SongFormat::~SongFormat() {
}

QString const& SongFormat::description() const {
    return m_description;
}

QUrl SongFormat::iconPath() const {
    return getIconPath(id());
}

QUrl SongFormat::getIconPath(int formatId) {
    QString appFolder(QDir::homePath());
    appFolder.chop(4); // remove data directory from end
    QString icons = QString("file://%1/%2").arg(appFolder).arg("app/native/assets/images/formats");
    switch(formatId){
    case 1:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("mod"));
    case 2:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("669"));
    case 3:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("it"));
    case 4:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("med"));
    case 5:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("mtm"));
    case 6:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("oct"));
    case 7:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("okt"));
    case 8:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("s3m"));
    case 9:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("stm"));
    case 10:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("xm"));
    case 11:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("ahx"));
    case 12:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("hvl"));
    case 13:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("mo3"));
    default:
        qDebug() << "Could not find icon for format id:" << formatId;
        return QUrl();
    }
}

QDebug operator << (QDebug dbg, SongFormat const &format) {
    dbg.nospace() << "(SongFormat:"
                  << "id="
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
