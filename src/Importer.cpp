#include "modplug/modplug.h"
#include <dirent.h>
#include <fnmatch.h>
#include <errno.h>
#include "Importer.hpp"
#include "FileUtils.hpp"
#include "Analytics.hpp"
#include "SongExtendedInfo.hpp"
#include "SongFormat.hpp"
#include "Catalog.hpp"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>
#include <QStack>
#include <QByteArray>
#include <bb/system/SystemProgressDialog>
#include "libid3tag/id3_id3tag.h"

#define DETAILED_LOG
#define REDUCED_SEARCH_SCOPE

using namespace bb::system;

Importer::Importer(QStringList const& filters,
                   Catalog * catalog,
                   QObject * parent)
    : QObject(parent),
      m_catalog(catalog),
      m_progress(NULL),
      m_filters(filters),
      m_numImportedSongs(0) {
}

Importer::~Importer() {
    destroyProgressUI();
}

int Importer::numImportedSongs() const {
    return m_numImportedSongs;
}

void Importer::clean() {
    //TODO: do not clean everything, clean only missing songs
    m_catalog->clearPersonalSongs();
}

int Importer::import()
{
    m_numImportedSongs = 0;

    clean();
    createProgressUI();
#ifdef REDUCED_SEARCH_SCOPE
    static const char * locations[] = {
        "shared/music",
    };
#else
    static const char * locations[] = {
        "shared/documents",
        "shared/downloads",
        "shared/music",
        "shared/Box",
        "shared/Dropbox",
        "removable/sdcard"
    };
#endif
    for(size_t i = 0; i < sizeof(locations)/sizeof(locations[0]); ++i) {
        const char * location = locations[i];
        QString path = FileUtils::joinPath("/accounts/1000", location);
        scanDirectory(QDir(path));
    }

    if(m_numImportedSongs == 0) {
        updateProgressUI(tr("No songs found"), 100);
    } else {
        updateProgressUI(QString(tr("Imported %1 songs")).arg(m_numImportedSongs), 100);
    }

    Analytics::getInstance()->importedSongCount(m_numImportedSongs);

    completeProgressUI();
    return m_numImportedSongs;
}

int Importer::scanDirectory(QDir const& root)
{
    QStack<QString> stack;
    stack.push(root.absolutePath());
    while(!stack.isEmpty()) {
        QString directoryPath = stack.pop();
        QString location = directoryPath;
        location.remove(0, 15);
        QString progressMessage = QString(tr("Searching for songs in %1...")).arg(location);
        updateProgressUI(progressMessage, INDEFINITE);
        QDir directory(directoryPath);
#if 0
        // This does not work because file system uses 64 bit inodes
        QStringList entries = directory.entryList(m_filters,
                                                  QDir::Files | QDir::Readable | QDir::CaseSensitive,
                                                  QDir::NoSort);
#endif
        QStringList entries;
        DIR *dirp;
        if ((dirp = opendir(directoryPath.toStdString().c_str())) != NULL) {
            struct dirent64 *dp;
            do {
                if ((dp = readdir64(dirp)) != NULL) {
                    if(dp->d_name[0] == '.' && (dp->d_name[1] == '\0' || (dp->d_name[1] == '.' && dp->d_name[2] == '\0')))
                        continue;
                    for(QStringList::const_iterator i = m_filters.begin(); i != m_filters.end(); ++i) {
                        if(fnmatch(i->toStdString().c_str(), dp->d_name, 0) == 0) {
                            entries << dp->d_name;
                            break;
                        }
                    }
                }
            } while (dp != NULL);
            closedir(dirp);
        }
        for(int i = 0; i < entries.size(); i++) {
            QString absoluteFileName = FileUtils::joinPath(directoryPath, entries[i]);
            QString extenion = FileUtils::extension(absoluteFileName).toLower();
            if(extenion == ".mp3") {
                importMp3File(absoluteFileName);
            } else {
                importTrackerSong(absoluteFileName);
            }
        }
        QFileInfoList infoEntries = directory.entryInfoList(QStringList(),
                                                            QDir::AllDirs | QDir::NoDotAndDotDot,
                                                            QDir::NoSort);
        for (int i = 0; i < infoEntries.size(); i++) {
            stack.push(infoEntries[i].absoluteFilePath());
        }
   }
   return m_numImportedSongs;
}

QString Importer::getMp3Attribute(void const * tag, const char * attributeName) {
    struct id3_frame const *frame = ::id3_tag_findframe((struct id3_tag const *)tag,
                                                        attributeName,
                                                        0);
    if (frame == 0)
        return "";

    union id3_field const *field = ::id3_frame_field(frame, 1);
    unsigned int nstrings = ::id3_field_getnstrings(field);

    for (unsigned int j = 0; j < nstrings; ++j) {
        id3_ucs4_t const *ucs4 = ::id3_field_getstrings(field, j);

        if(strcmp(attributeName, ID3_FRAME_GENRE) == 0)
            ucs4 = ::id3_genre_name(ucs4);

        id3_latin1_t *latin1 = ::id3_ucs4_latin1duplicate(ucs4);
        if (latin1 == 0)
            return "";

        QString result = QString((const char*)latin1);
        free(latin1);
        return result.trimmed();
    }
    return "";
}

bool Importer::importMp3File(QString const& fileName) {
    QString fileNameOnly = FileUtils::fileNameOnly(fileName);
    QString progressMessage = QString(tr("Importing %1")).arg(fileNameOnly);
    updateProgressUI(progressMessage, INDEFINITE);

    ::id3_file * mp3file = ::id3_file_open(fileName.toStdString().c_str(), ID3_FILE_MODE_READONLY);
    if(mp3file == NULL) {
        qDebug() << "Could not open Mp3 file" << fileName;
        return false;
    }

    ::id3_tag * tag = ::id3_file_tag(mp3file);
    if(tag != NULL) {
        ++m_numImportedSongs;
        SongExtendedInfo info(NULL);
        info.setId(-m_numImportedSongs);
        info.setFileName(fileName);
        {
            QFile file(fileName);
            info.setFileSize(file.size());
        }
        info.setInstruments(0);
        info.setChannels(0);
        info.setSamples(0);
        info.setPatterns(0);
        info.setOrders(0);

        info.setFormatId(SongFormat::getFormatIdByFileName(fileName));
        info.setFormat("MP3 format");

        info.setSongLength(60);

        QString title = getMp3Attribute(tag, ID3_FRAME_TITLE);
        info.setTitle(title);

        info.setTracker("");
        info.setTrackerId(0);

        QString track = getMp3Attribute(tag, ID3_FRAME_TRACK);

        // remove leading zeroes
        while(track.length() > 1 && track[0] == '0') {
            track = track.mid(1);
        }
        int trackId = -1;
        if(track.length() > 0) {
            bool bOk = false;
            trackId = track.toInt(&bOk, 10);
            if(!bOk) {
                trackId = -1;
            }
        }
        QString year = getMp3Attribute(tag, ID3_FRAME_YEAR);

        QString album = getMp3Attribute(tag, ID3_FRAME_ALBUM);
        int albumId = 0;
        if(album.length() > 0) {
            albumId = m_catalog->createAlbum(album);
            m_catalog->addSongToAlbum(albumId, -m_numImportedSongs, trackId);
        }

        int genreId = 0;
        QString genre = getMp3Attribute(tag, ID3_FRAME_GENRE);
        if(genre.length() > 0) {
            genreId = m_catalog->createGenre(genre);
        }
        info.setGenre(genre);
        info.setGenreId(genreId);

        int artistId = 0;
        QString artist = getMp3Attribute(tag, ID3_FRAME_ARTIST);
        if(artist.length() > 0) {
            artistId = m_catalog->createArtist(artist);
        }
        info.setArtistId(artistId);
        info.setArtist(artist);

        m_catalog->addPersonalSong(info);

#ifdef DETAILED_LOG
        qDebug() << "Tags:" << "Title=" << title << "Artist=" << artist << "Album=" << album << "Track=" << track << "Year=" << year << "Genre=" << genre;
#endif
    }

    ::id3_file_close(mp3file);
    return false;
}

bool Importer::importTrackerSong(QString const& fileName)
{
    QString fileNameOnly = FileUtils::fileNameOnly(fileName);
    QString progressMessage = QString(tr("Importing %1")).arg(fileNameOnly);
    updateProgressUI(progressMessage, INDEFINITE);

    QFile inputFile(fileName);
    if(!inputFile.exists()) {
        qDebug() << "File does not exist" << fileName;
        return false;
    }

    if(!inputFile.open(QFile::ReadOnly)) {
        qDebug() << "Failed to open input file" << fileName;
        return false;
    }

    QByteArray data = inputFile.readAll();
    if(data.size() == 0) {
        qDebug() << "Failed to read file" << fileName;
        return false;
    }

    ModPlugFile* module = ::ModPlug_Load(data.data(), data.size());
    if(module == NULL) {
        qDebug() << "Failed to load tracker module" << fileName;
        return false;
    }

    ++m_numImportedSongs;
    SongExtendedInfo info(NULL);
    info.setId(-m_numImportedSongs);
    info.setFileName(fileName);
    info.setFileSize(data.size());
    info.setSongLength(::ModPlug_GetLength(module));
    info.setTitle(::ModPlug_GetName(module));

    info.setInstruments(::ModPlug_NumInstruments(module));
    info.setChannels(::ModPlug_NumChannels(module));
    info.setSamples(::ModPlug_NumSamples(module));
    info.setPatterns(::ModPlug_NumPatterns(module));
    info.setOrders(::ModPlug_NumOrders(module));

    ::ModPlug_Unload(module);

    info.setFormatId(SongFormat::getFormatIdByFileName(fileName));
    info.setFormat("");

    info.setTrackerId(0);
    info.setTracker("");

    info.setGenreId(0);
    info.setGenre("");

    info.setArtistId(0);
    info.setArtist("");

    m_catalog->addPersonalSong(info);
    return true;
}

void Importer::destroyProgressUI() {
    if(m_progress != NULL) {
        m_progress->cancel();
        delete m_progress;
        m_progress = NULL;
    }
}

void Importer::createProgressUI() {
    destroyProgressUI();
    m_progress = new SystemProgressDialog(0);
    m_progress->setModality(SystemUiModality::Application);
    m_progress->setState(SystemUiProgressState::Active);
    m_progress->setTitle(tr("Importing Songs"));
    m_progress->confirmButton()->setEnabled(false);
    m_progress->setProgress(INDEFINITE);
    m_progress->show();
}

void Importer::completeProgressUI() {
    if(m_progress) {
        m_progress->confirmButton()->setEnabled(true);
        m_progress->exec();
    }
}

void Importer::updateProgressUI(QString const& body, int progress) {
#ifdef DETAILED_LOG
    qDebug() << body;
#endif
    if(m_progress) {
        m_progress->setBody(body);
        m_progress->setProgress(progress);
        m_progress->show();
    }
}
