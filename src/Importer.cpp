#include "Importer.hpp"
#include "FileUtils.hpp"
#include "FileSelector.hpp"
#include "Analytics.hpp"
#include "SongExtendedInfo.hpp"
#include "SongFormat.hpp"
#include "Catalog.hpp"
#include <QByteArray>
#include "modplug/modplug.h"
#include "taglib/taglib.h"
#include "taglib/fileref.h"
#include "taglib/tag.h"
#include "taglib/audioproperties.h"
#include "taglib/tpropertymap.h"

#ifdef _DEBUG
//#define VERBOSE_LOGGING
#else
#endif

using namespace bb::system;

Importer::Importer(QStringList const& filters,
                   Catalog * catalog,
                   QObject * parent)
    : QObject(parent),
      m_filters(filters),
      m_catalog(catalog),
      m_messageBox(tr("Importing Songs and Playlists"), ""),
      m_numImportedSongs(0),
      m_numImportedPlaylists(0),
      m_nextId(-1) {
}

Importer::~Importer() {
#ifdef VERBOSE_LOGGING
    qDebug() << "Importer::~Importer()";
#endif
}

int Importer::numImportedSongs() const {
    return m_numImportedSongs;
}

int Importer::numImportedPlaylists() const {
    return m_numImportedPlaylists;
}

void Importer::removeMissingSongs() {
    m_messageBox.setBody(tr("Removing missing songs from library..."));

    int missingSongs = 0;
    m_nextId = -1;
    m_knownFileNames.clear();
    std::vector<LocalSongInfo> songs;
    if(m_catalog->getLocalSongs(songs)) {
        qDebug() << "Local songs:" << songs.size();
        for(size_t i = 0; i < songs.size(); ++i) {
            if(!FileUtils::exists(songs[i].filePath())) {
                qDebug() << "Missing song: id:" << songs[i].id() << ", path:" << songs[i].filePath();
                QString fileNameOnly = FileUtils::fileNameOnly(songs[i].filePath());
                m_messageBox.setBody(tr("Removing missing %1").arg(fileNameOnly));
                m_catalog->deleteSong(songs[i].id());
                missingSongs++;
            } else {
                m_knownFileNames.insert(m_knownFileNames.begin(), songs[i].filePath());
            }
            m_nextId = std::min(songs[i].id() - 1, m_nextId);
        }
    }

    m_messageBox.setBody(tr("Updating songs library..."));
    m_catalog->houseKeep();
}

bool Importer::lastImportPerformed(QDateTime &date) {
    QFile file(FileUtils::joinPath(QDir::homePath(), ".last_import"));
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream input(&file);
        if(!file.atEnd()) {
            date = QDateTime::fromString(input.readLine(), Qt::ISODate);
            return date.isValid();
        }
    }
    return false;
}

void Importer::start() {
    m_numImportedSongs = 0;
    m_numImportedPlaylists = 0;
    m_nextId = -1;

    removeMissingSongs();

    FileSelector * selector = new FileSelector(m_filters);

    bool rc;
    Q_UNUSED(rc);
    rc = QObject::connect(selector, SIGNAL(foundFile(QString const&)),
                          this,     SLOT(onFoundFile(QString const&)),
                          Qt::QueuedConnection);
    Q_ASSERT(rc);
    rc = QObject::connect(selector, SIGNAL(foundPlaylist(QString const&, QVector<QString> const&)),
                          this,     SLOT(onFoundPlaylist(QString const&, QVector<QString> const&)),
                          Qt::QueuedConnection);
    Q_ASSERT(rc);
    rc = QObject::connect(selector, SIGNAL(searchingDirectory(QString const&)),
                          this,     SLOT(onSearchingDirectory(QString const&)),
                          Qt::QueuedConnection);
    Q_ASSERT(rc);
    rc = QObject::connect(selector, SIGNAL(done()),
                          this,     SLOT(onSearchCompleted()),
                          Qt::QueuedConnection);
    Q_ASSERT(rc);

    selector->start();
}

void Importer::onSearchCompleted() {
    if(m_numImportedSongs == 0 && m_numImportedPlaylists == 0) {
        m_messageBox.setBody(tr("No new songs or playlists found")).setProgress(100);
    } else {
        if(m_numImportedPlaylists == 0) {
            m_messageBox.setBody(tr("Imported %1 song(s)").arg(m_numImportedSongs)).setProgress(100);
        } else if(m_numImportedSongs == 0) {
            m_messageBox.setBody(tr("Imported %1 playlist(s)").arg(m_numImportedPlaylists)).setProgress(100);
        } else {
            m_messageBox.setBody(tr("Imported %1 song(s) and %2 playlist(s)").arg(m_numImportedSongs).arg(m_numImportedPlaylists)).setProgress(100);
        }
    }
    m_knownFileNames.clear();

    Analytics::getInstance()->importedSongCount(m_numImportedSongs);
    Analytics::getInstance()->importedPlaylistCount(m_numImportedPlaylists);

    updateLastImportedInfo();

    m_messageBox.enableButton(true);
    m_messageBox.run();
    emit searchCompleted();
}

void Importer::updateLastImportedInfo() {
    QString infoFile(FileUtils::joinPath(QDir::homePath(), ".last_import"));
    QFile file(infoFile);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream(&file) << QDateTime::currentDateTime().toString(Qt::ISODate);
    }
}

void Importer::onSearchingDirectory(QString const& location) {
    m_messageBox.setBody(tr("Searching for songs and playlists in %1...").arg(location));
}

void Importer::onFoundFile(QString const& fileName) {
    if(m_knownFileNames.find(fileName) != m_knownFileNames.end()) {
        return; // already exists
    }
    if(SongFormat::isTrackerSong(fileName)) {
        importTrackerSong(fileName);
    } else {
        importTaggedSong(fileName);
    }
}

void Importer::onFoundPlaylist(QString const& playlistName,
                               QVector<QString> const& songs) {
#ifdef VERBOSE_LOGGING
    qDebug() << "Found playlist" << playlistName << "with" << songs << "songs";
#endif
    if(songs.empty()) {
        return;
    }

    m_messageBox.setBody(tr("Processing playlist %1...").arg(playlistName));

    m_catalog->deletePlaylistByName(playlistName);

    const int playlistId = m_catalog->createPlaylist(playlistName);
    if(playlistId == 0) {
#ifdef VERBOSE_LOGGING
        qDebug() << "Could not insert playlist" << playlistName;
#endif
        return;
    }

    m_numImportedPlaylists++;

    foreach(QString const songFileName, songs) {
        const int songId = m_catalog->resolveModuleIdByFileName(songFileName);
        if(songId == 0) {
#ifdef VERBOSE_LOGGING
            qDebug() << "Could not resolve song" << songFileName;
#endif
        } else {
            m_catalog->appendToPlaylist(playlistId, songId);
        }
    }
}

bool Importer::importTaggedSong(QString const& fileName)
{
    QString fileNameOnly = FileUtils::fileNameOnly(fileName);
    m_messageBox.setBody(tr("Importing %1").arg(fileNameOnly));

    SongExtendedInfo info(NULL);
    info.setId(m_nextId--);
    info.setFileName(fileName);
    info.setFormat(SongFormat::getFormatByFileName(fileName));
    info.setIconPath(SongFormat::getIconPath(fileName));
    {
        QFile file(fileName);
        info.setFileSize(file.size());
    }
    info.setInstruments(0);
    info.setChannels(0);
    info.setSamples(0);
    info.setPatterns(0);
    info.setOrders(0);

    TagLib::FileRef f(fileName.toUtf8().constData(),
                      true,
                      TagLib::AudioProperties::Accurate);

    unsigned long milliseconds;
    if(!f.isNull() && f.audioProperties()) {
        TagLib::AudioProperties *properties = f.audioProperties();
        milliseconds = properties->length() * 1000;
    } else {
        milliseconds = 0;
    }

    info.setSongLength(milliseconds);
    info.setTracker("");
    info.setTrackerId(0);

    if(!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();

        std::string title(tag->title().to8Bit(true));
        info.setTitle(QString::fromUtf8(title.data(), title.length()));

        int artistId;
        std::string artistTemp(tag->artist().to8Bit(true));
        QString artist = QString::fromUtf8(artistTemp.data(), artistTemp.length());
        info.setArtist(artist);
        if(artist.length() > 0) {
            artistId = m_catalog->createArtist(artist);
        } else {
            artistId = 0;
        }
        info.setArtistId(artistId);

        int genreId;
        std::string genreTemp(tag->genre().to8Bit(true));
        QString genre = QString::fromUtf8(genreTemp.data(), genreTemp.length());
        if(genre.length() > 0) {
            genreId = m_catalog->createGenre(genre);
        } else {
            genreId = 0;
        }
        info.setGenre(genre);
        info.setGenreId(genreId);

        int trackId = tag->track();

        int albumId;
        std::string albumTemp(tag->album().to8Bit(true));
        QString album = QString::fromUtf8(albumTemp.data(), albumTemp.length());
        if(album.length() > 0) {
            albumId = m_catalog->createAlbum(artist, album);
            m_catalog->addSongToAlbum(albumId, info.id(), trackId);
        } else {
            albumId = 0;
        }
    } else {
        info.setTitle(fileNameOnly);
        info.setArtist("- N/A -");
        info.setArtistId(0);
        info.setGenre("- N/A -");
        info.setGenreId(0);
    }

    m_catalog->addPersonalSong(info);
    ++m_numImportedSongs;
    return true;
}

bool Importer::importTrackerSong(QString const& fileName)
{
    QString fileNameOnly = FileUtils::fileNameOnly(fileName);
    m_messageBox.setBody(tr("Importing %1").arg(fileNameOnly));

    QFile inputFile(fileName);
    if(!FileUtils::exists(fileName)) {
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

    SongExtendedInfo info(NULL);
    info.setId(m_nextId--);
    info.setFileName(fileName);
    info.setIconPath(SongFormat::getIconPath(fileName));
    info.setFileSize(data.size());
    info.setSongLength(::ModPlug_GetLength(module));
    info.setTitle(::ModPlug_GetName(module));

    info.setInstruments(::ModPlug_NumInstruments(module));
    info.setChannels(::ModPlug_NumChannels(module));
    info.setSamples(::ModPlug_NumSamples(module));
    info.setPatterns(::ModPlug_NumPatterns(module));
    info.setOrders(::ModPlug_NumOrders(module));

    ::ModPlug_Unload(module);

    info.setFormat("");

    info.setTrackerId(0);
    info.setTracker("");

    info.setGenreId(0);
    info.setGenre("");

    info.setArtistId(0);
    info.setArtist("");

    m_catalog->addPersonalSong(info);
    ++m_numImportedSongs;
    return true;
}
