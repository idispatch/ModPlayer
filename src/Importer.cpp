#include "Importer.hpp"
#include "FileUtils.hpp"
#include "FileSelector.hpp"
#include "Analytics.hpp"
#include "SongExtendedInfo.hpp"
#include "SongFormat.hpp"
#include "Catalog.hpp"
#include <QByteArray>
#include "modplug/modplug.h"
#include "libid3tag/id3_id3tag.h"
#include "libid3tag/id3_utf8.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

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
    if(FileSelector::isMp3(fileName)) {
        importMp3File(fileName);
    } else {
        importTrackerSong(fileName);
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
        id3_utf8_t *utf8 = ::id3_ucs4_utf8duplicate(ucs4);
        if (utf8 == 0)
            return "";
        QString result = QString::fromUtf8(reinterpret_cast<const char*>(utf8));
        free(utf8);
        return result.trimmed();
    }
    return "";
}

bool Importer::importMp3File(QString const& fileName) {
    QString fileNameOnly = FileUtils::fileNameOnly(fileName);
    m_messageBox.setBody(tr("Importing %1").arg(fileNameOnly));

    ::id3_file * mp3file = ::id3_file_open(fileName.toUtf8().constData(), ID3_FILE_MODE_READONLY);
    if(mp3file == NULL) {
        return false;
    }

    ::id3_tag * tag = ::id3_file_tag(mp3file);
    if(tag != NULL)
    {
        SongExtendedInfo info(NULL);
        info.setId(m_nextId--);
        info.setFileName(fileName);
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

        mad_timer_t duration;
        signed int kbps;
        unsigned long kbytes;
        mad_timer_reset(&duration);
        int result = calculateMp3Duration(fileName.toUtf8().constData(), &duration, &kbps, &kbytes);
        if(result == 0)
        {
            duration = ::mad_timer_abs(duration);
            unsigned long milliseconds = duration.seconds * 1000 + duration.fraction * 1000/MAD_TIMER_RESOLUTION;
            info.setSongLength(milliseconds);
        }

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
            int pos = 0;
            while(pos < track.length()) {
                if(track[pos] >= '0' && track[pos] <= '9') {
                    pos++;
                } else {
                    break;
                }
            }
            track = track.left(pos);
            bool bOk = false;
            trackId = track.toInt(&bOk, 10);
            if(!bOk) {
                trackId = -1;
            }
        }

        QString year = getMp3Attribute(tag, ID3_FRAME_YEAR);

        int artistId = 0;
        QString artist = getMp3Attribute(tag, ID3_FRAME_ARTIST);
        if(artist.length() > 0) {
            artistId = m_catalog->createArtist(artist);
        }
        info.setArtistId(artistId);
        info.setArtist(artist);

        QString album = getMp3Attribute(tag, ID3_FRAME_ALBUM);
        int albumId = 0;
        if(album.length() > 0) {
            albumId = m_catalog->createAlbum(artist, album);
            m_catalog->addSongToAlbum(albumId, info.id(), trackId);
        }

        int genreId = 0;
        QString genre = getMp3Attribute(tag, ID3_FRAME_GENRE);
        if(genre.length() > 0) {
            genreId = m_catalog->createGenre(genre);
        }
        info.setGenre(genre);
        info.setGenreId(genreId);

        m_catalog->addPersonalSong(info);
        ++m_numImportedSongs;
#ifdef VERBOSE_LOGGING
        qDebug() << "Tags:" << "Title=" << title << "Artist=" << artist << "Album=" << album << "Track=" << track << "Year=" << year << "Genre=" << genre;
#endif
    }

    ::id3_file_close(mp3file);
    return false;
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

int Importer::calculateMp3Duration(char const *path,
                                   mad_timer_t *duration,
                                   signed int *kbps,
                                   unsigned long *kbytes) {
    int fd = ::open(path, O_RDONLY | O_BINARY);
    if (fd == -1) {
        qDebug() << "Could not open file" << path;
        return -1;
    }

    struct stat stat;
    if (::fstat(fd, &stat) == -1) {
        qDebug() << "Could not fstat file" << path;
        close(fd);
        return -1;
    }

    if (!S_ISREG(stat.st_mode)) {
        qDebug() << "Not a regular file" << path;
        close(fd);
        return -1;
    }

    if(kbytes) {
        *kbytes = (stat.st_size + 512) / 1024;
    }

#if 0
    void * fdm = ::mmap(0, stat.st_size, PROT_READ, MAP_PRIVATE | MAP_LAZY | MAP_NOINIT, fd, 0);
#else
    void * fdm = ::mmap(0, stat.st_size, PROT_READ, MAP_PRIVATE | MAP_NOINIT, fd, 0);
#endif
    if (fdm == MAP_FAILED) {
        qDebug() << "Could not mmap" << stat.st_size << "bytes for file" << path;
        close(fd);
        return -1;
    }

    if (fdm) {
        int result = scanMp3(reinterpret_cast<unsigned char const*>(fdm), stat.st_size, duration);
        if(kbps) {
            *kbps = result;
        }

        if(::munmap(fdm, stat.st_size) == -1) {
            qDebug() << "Could not munmap" << stat.st_size << "bytes for file" << path;
            close(fd);
            return -1;
        }
    } else {
        if(kbps) {
            *kbps = 0;
        }
    }

    if (::close(fd) == -1) {
        qDebug() << "Could not close file" << path;
        return -1;
    }

    return 0;
}

int Importer::scanMp3(unsigned char const *ptr,
                      unsigned long len,
                      mad_timer_t *duration) {
    struct mad_stream stream;
    struct mad_header header;

    ::mad_stream_init(&stream);
    ::mad_header_init(&header);
    ::mad_stream_buffer(&stream, ptr, len);

    unsigned long bitrate = 0;
    unsigned long kbps = 0;
    unsigned long count = 0;
    int vbr = 0;

    while (1) {
        if (::mad_header_decode(&header, &stream) == -1) {
            if (MAD_RECOVERABLE(stream.error))
                continue;
            else
                break;
        }

        if (bitrate && header.bitrate != bitrate)
            vbr = 1;

        bitrate = header.bitrate;

        kbps += bitrate / 1000;
        ++count;

        ::mad_timer_add(duration, header.duration);
    }
    mad_header_finish(&header);
    ::mad_stream_finish(&stream);

    if (count == 0)
        count = 1;

    return ((kbps * 2) / count + 1) / 2 * (vbr ? -1 : 1);
}
