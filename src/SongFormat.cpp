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
                       int duration,
                       QObject *parent)
    : ItemGroupBase(id, name, count, duration, parent),
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
    case FORMAT_MOD:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("mod"));
    case FORMAT_669:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("669"));
    case FORMAT_IT:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("it"));
    case FORMAT_MED:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("med"));
    case FORMAT_MTM:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("mtm"));
    case FORMAT_OCT:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("oct"));
    case FORMAT_OKT:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("okt"));
    case FORMAT_S3M:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("s3m"));
    case FORMAT_STM:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("stm"));
    case FORMAT_XM:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("xm"));
    case FORMAT_AHX:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("ahx"));
    case FORMAT_HVL:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("hvl"));
    case FORMAT_MP3:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("mp3"));
    default:
        return QUrl(QString("%1/icon_unknown.png").arg(icons));
    }
}

int SongFormat::getFormatIdByFileName(QString const& fileName) {
    if(fileName.endsWith(".mod", Qt::CaseInsensitive)) {
        return FORMAT_MOD;
    }
    if(fileName.endsWith(".669", Qt::CaseInsensitive)) {
        return FORMAT_669;
    }
    if(fileName.endsWith(".it", Qt::CaseInsensitive)) {
        return FORMAT_IT;
    }
    if(fileName.endsWith(".med", Qt::CaseInsensitive)) {
        return FORMAT_MED;
    }
    if(fileName.endsWith(".mtm", Qt::CaseInsensitive)) {
        return FORMAT_MTM;
    }
    if(fileName.endsWith(".oct", Qt::CaseInsensitive)) {
        return FORMAT_OCT;
    }
    if(fileName.endsWith(".okt", Qt::CaseInsensitive)) {
        return FORMAT_OKT;
    }
    if(fileName.endsWith(".s3m", Qt::CaseInsensitive)) {
        return FORMAT_S3M;
    }
    if(fileName.endsWith(".stm", Qt::CaseInsensitive)) {
        return FORMAT_STM;
    }
    if(fileName.endsWith(".xm", Qt::CaseInsensitive)) {
        return FORMAT_XM;
    }
    if(fileName.endsWith(".ahx", Qt::CaseInsensitive)) {
        return FORMAT_AHX;
    }
    if(fileName.endsWith(".hvl", Qt::CaseInsensitive)) {
        return FORMAT_HVL;
    }
    if(fileName.endsWith(".mo3", Qt::CaseInsensitive)) {
        return FORMAT_MO3;
    }
    if(fileName.endsWith(".mp3", Qt::CaseInsensitive)) {
        return FORMAT_MP3;
    }
    return 0;
}

bool SongFormat::isTrackerSong(int formatId) {
    switch(formatId) {
    case FORMAT_MOD:
    case FORMAT_669:
    case FORMAT_IT:
    case FORMAT_MED:
    case FORMAT_MTM:
    case FORMAT_OCT:
    case FORMAT_OKT:
    case FORMAT_S3M:
    case FORMAT_STM:
    case FORMAT_XM:
        return true;
    default:
        return false;
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
