#include "SongFormat.hpp"
#include <QDebug>
#include <QDir>

template<>
int InstanceCounter<SongFormat>::s_count;
template<>
int InstanceCounter<SongFormat>::s_maxCount;

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
        return QUrl(QString("%1/icon_unknown.png").arg(icons));
    }
}

int SongFormat::getFormatIdByFileName(QString const& fileName) {
    if(fileName.endsWith(".mod", Qt::CaseInsensitive)) {
        return 1;
    }
    if(fileName.endsWith(".669", Qt::CaseInsensitive)) {
        return 2;
    }
    if(fileName.endsWith(".it", Qt::CaseInsensitive)) {
        return 3;
    }
    if(fileName.endsWith(".med", Qt::CaseInsensitive)) {
        return 4;
    }
    if(fileName.endsWith(".mtm", Qt::CaseInsensitive)) {
        return 5;
    }
    if(fileName.endsWith(".oct", Qt::CaseInsensitive)) {
        return 6;
    }
    if(fileName.endsWith(".okt", Qt::CaseInsensitive)) {
        return 7;
    }
    if(fileName.endsWith(".s3m", Qt::CaseInsensitive)) {
        return 8;
    }
    if(fileName.endsWith(".stm", Qt::CaseInsensitive)) {
        return 9;
    }
    if(fileName.endsWith(".xm", Qt::CaseInsensitive)) {
        return 10;
    }
    if(fileName.endsWith(".ahx", Qt::CaseInsensitive)) {
        return 11;
    }
    if(fileName.endsWith(".hvl", Qt::CaseInsensitive)) {
        return 12;
    }
    if(fileName.endsWith(".mo3", Qt::CaseInsensitive)) {
        return 1;
    }
    return 0;
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
