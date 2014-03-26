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
                       double duration,
                       QObject *parent)
    : ItemGroupBase(id, name, count, duration, parent),
      m_description(description) {
}

SongFormat::~SongFormat() {
#ifdef VERBOSE_LOGGING
    qDebug() << "SongFormat::~SongFormat()";
#endif
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
    case FORMAT_OGG:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("ogg"));
    case FORMAT_FLAC:
        return QUrl(QString("%1/icon_%2.png").arg(icons).arg("flac"));
    default:
        return QUrl(QString("%1/icon_unknown.png").arg(icons));
    }
}

QString SongFormat::getFormatByFormatId(int formatId) {
    switch(formatId){
    case FORMAT_MOD:
        return "Amiga module file";
    case FORMAT_669:
        return "669 Mod Composer Module";
    case FORMAT_IT:
        return "Impulse Tracker Module";
    case FORMAT_MED:
        return "OctaMED Module";
    case FORMAT_MTM:
        return "MultiTracker Module";
    case FORMAT_OCT:
        return "OctaMED Module";
    case FORMAT_OKT:
        return "Oktalyzer Module";
    case FORMAT_S3M:
        return "ScreamTracker 3 Module";
    case FORMAT_STM:
        return "ScreamTracker 2 Module";
    case FORMAT_XM:
        return "FastTracker 2 Module";
    case FORMAT_AHX:
        return "Amiga Chiptune Module";
    case FORMAT_HVL:
        return "HivelyTracker Module";
    case FORMAT_MP3:
        return "MP3 Song";
    case FORMAT_OGG:
        return "Xiph.org Foundation OGG Song";
    case FORMAT_FLAC:
        return "Free Lossless Audio Codec Song";
    default:
        return "Unknown format";
    }
}

int SongFormat::getFormatIdByFileName(QString const& fileName) {
    int len = fileName.length();
    if(len > 0) {
        switch(fileName[len - 1].unicode()) {
        case 'c': case 'C': // .flac
            if(len > 1) {
                switch(fileName[len - 2].unicode()) {
                case 'a': case 'A':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'l': case 'L':
                            if(len > 3) {
                                switch(fileName[len - 4].unicode()) {
                                case 'f': case 'F':
                                    if(len > 4 && fileName[len - 5] == '.') {
                                        return FORMAT_FLAC;
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        case 'x': case 'X': // .ahx
            if(len > 1) {
                switch(fileName[len - 2].unicode()) {
                case 'h': case 'H':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'a': case 'A':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_AHX;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        case 'l': case 'L': // .hvl
            if(len > 1) {
                switch(fileName[len - 2].unicode()) {
                case 'v': case 'V':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'h': case 'H':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_HVL;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        case 'g': case 'G': // .ogg
            if(len > 1) {
                switch(fileName[len - 2].unicode()) {
                case 'g': case 'G':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'o': case 'O':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_OGG;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        case 'd': case 'D': // .mod, .med
            if(len > 1) {
                switch(fileName[len - 2].unicode()) {
                case 'o': case 'O':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'm': case 'M':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_MOD;
                            }
                            break;
                        }
                    }
                    break;
                case 'e': case 'E':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'm': case 'M':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_MED;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        case 't': case 'T': // .it .oct .okt
            if(len > 1) {
                switch(fileName[len - 2].unicode()) {
                case 'i': case 'I':
                    if(len > 2 && fileName[len - 3] == '.') {
                        return FORMAT_IT;
                    }
                    break;
                case 'c': case 'C':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'o': case 'O':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_OCT;
                            }
                            break;
                        }
                    }
                    break;
                case 'k': case 'K':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'o': case 'O':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_OKT;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        case '3': // .mo3 .mp3
            if(len > 1) {
                switch(fileName[len - 2].unicode()) {
                case 'o': case 'O':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'm': case 'M':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_MO3;
                            }
                            break;
                        }
                    }
                    break;
                case 'p': case 'P':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'm': case 'M':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_MP3;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        case '9': // .669
            if(len > 1) {
                switch(fileName[len - 2].unicode()) {
                case '6':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case '6':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_669;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        case 'm': case 'M': // .mtm .stm .s3m .xm
            if(len > 1) {
                switch(fileName[len - 2].unicode()) {
                case 'x': case 'X':
                    if(len > 2 && fileName[len - 3] == '.') {
                        return FORMAT_XM;
                    }
                    break;
                case 't': case 'T':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 'm': case 'M':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_MTM;
                            }
                            break;
                        case 's': case 'S':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_STM;
                            }
                            break;
                        }
                    }
                    break;
                case '3':
                    if(len > 2) {
                        switch(fileName[len - 3].unicode()) {
                        case 's': case 'S':
                            if(len > 3 && fileName[len - 4] == '.') {
                                return FORMAT_S3M;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
    return FORMAT_UNKNOWN;
}

bool SongFormat::isMp3Song() const {
    return id() == FORMAT_MP3;
}

bool SongFormat::isMp3Song(QString const& fileName) {
    return SongFormat::isMp3Song(SongFormat::getFormatIdByFileName(fileName));
}

bool SongFormat::isMp3Song(int formatId) {
    return formatId == FORMAT_MP3;
}

bool SongFormat::isTrackerSong() const {
    return SongFormat::isTrackerSong(id());
}

bool SongFormat::isTrackerSong(QString const& fileName) {
    return SongFormat::isTrackerSong(SongFormat::getFormatIdByFileName(fileName));
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
                  << "id=" << format.id()
                  << ",name=" << format.name()
                  << ",description=" << format.description()
                  << ",count=" << format.count()
                  << ",duration=" << format.duration()
                  << ")";
    return dbg.space();
}
