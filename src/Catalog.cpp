#include "FileUtils.hpp"
#include "Catalog.hpp"
#include "Analytics.hpp"
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <bb/data/SqlDataAccess>
#include <bb/data/DataSource>

#include "SqlReader.hpp"

using namespace bb::data;
using namespace bb::cascades;

template<>
int InstanceCounter<Catalog>::s_count;
template<>
int InstanceCounter<Catalog>::s_maxCount;

static const char * SELECT_FROM_SONGS =
        "SELECT"
        " id,"
        " fileName,"
        " title,"
        " format,"
        " downloads,"
        " favourited,"
        " score,"
        " size,"
        " length,"
        " playCount,"
        " lastPlayed,"
        " myFavourite "
        "FROM songs";

int Catalog::Command::s_commandCounter = 0;
int Catalog::Version = 3;

Catalog::Catalog(QObject * parent)
    : QThread(parent),
      m_dataAccess(NULL) {
    initCatalog();
}

Catalog::~Catalog() {
#ifdef VERBOSE_LOGGING
    qDebug() << "Catalog::~Catalog()";
#endif
}

void Catalog::stopThread() {
    if(isRunning())
    {
        {
            QMutexLocker locker(&m_mutex);
            m_commandQueue.enqueue(new Command(Command::ExitCommand));
            m_cond.wakeOne();
        }
        QThread::wait(); // wait till thread is stopped
    }
}

int Catalog::songCount() {
    const char * query = "SELECT COUNT(id) FROM songs WHERE id > 0";
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    if(sqlQuery.next()) {
        return sqlQuery.value(0).toInt();
    }
    return 0;
}

int Catalog::personalSongCount() {
    const char * query = "SELECT COUNT(id) FROM songs WHERE id < 0";
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    if(sqlQuery.next()) {
        return sqlQuery.value(0).toInt();
    }
    return 0;
}

bool Catalog::getLocalSongs(std::vector<LocalSongInfo> & songs) {
    songs.clear();
    const char * query = "SELECT id, fileName FROM songs WHERE id < 0";
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        SqlReader reader(sqlQuery);
        int id;
        QString fileName;
        reader >> id >> fileName;
        songs.push_back(LocalSongInfo().setId(id).setFilePath(fileName));
    }
    return true;
}

ArrayDataModel* Catalog::findFormats() {
    const char * query =
            "SELECT"
            " formats.id AS id, "
            " formats.name AS name, "
            " formats.description AS description, "
            " COUNT(formats.id) AS songCount, "
            " SUM(songs.length) AS duration "
            "FROM formats "
            "INNER JOIN songs ON songs.format=formats.id "
            "GROUP BY formats.id";
    ArrayDataModel * model = new ArrayDataModel();
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        SqlReader reader(sqlQuery);
        int id;
        QString name;
        QString desc;
        int songCount;
        double totalDuration;
        reader >> id >> name >> desc >> songCount >> totalDuration;
        QObject* value = new SongFormat(id,
                                        name,
                                        desc,
                                        songCount,
                                        totalDuration,
                                        model);
        QVariant v = QVariant::fromValue(value);
        model->append(v);
    }
    return model;
}

GroupDataModel* Catalog::findGenres() {
    const char * query =
            "SELECT"
            " genres.id,"
            " genres.name,"
            " COUNT(songs.id) AS songCount, "
            " SUM(songs.length) AS duration "
            "FROM genres "
            "LEFT JOIN songs ON songs.genre=genres.id "
            "GROUP BY genres.id";
    GroupDataModel * model = new GroupDataModel(QStringList() << "name");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        SqlReader reader(sqlQuery);
        int id;
        QString name;
        int songCount;
        double totalDuration;
        reader >> id >> name >> songCount >> totalDuration;
        QObject* value = new SongGenre(id,
                                       name,
                                       songCount,
                                       totalDuration,
                                       model);
        model->insert(value);
    }
    return model;
}

GroupDataModel* Catalog::findArtists(QString const& searchTerm) {
    const char * query =
            "SELECT"
            " artists.id,"
            " artists.name,"
            " artists.score,"
            " artists.downloads,"
            " artists.rating,"
            " COUNT(songs.id) AS count, "
            " SUM(songs.length) AS duration "
            "FROM artists "
            "LEFT JOIN songs ON songs.artist=artists.id ";
    QString whereClause;
    if(searchTerm.length() > 0) {
        QString expr = QString(searchTerm).replace("'", "''")
                                          .replace("\\", "\\\\")
                                          .replace("%", "\\%")
                                          .replace("_", "\\_");
        whereClause += QString(" WHERE (artists.name LIKE '%%%1%%' ESCAPE '\\') ").arg(expr);
    }

    QString fullSql(query);
    fullSql += whereClause;
    fullSql += " GROUP BY artists.id";

    GroupDataModel * model = new GroupDataModel(QStringList() << "name");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(fullSql);
    while(sqlQuery.next()) {
        SqlReader reader(sqlQuery);
        int id;
        QString name;
        int score;
        int downloads;
        int rating;
        int songCount;
        double totalDuration;
        reader >> id >> name >> score >> downloads >> rating >> songCount >> totalDuration;
        QObject *value = new Artist(id,
                                    name,
                                    score,
                                    downloads,
                                    rating,
                                    songCount,
                                    totalDuration,
                                    model);
        model->insert(value);
    }
    return model;
}

GroupDataModel* Catalog::findPlaylists() {
    const char * query = "SELECT"
                         " playlists.id AS id,"
                         " playlists.name AS name,"
                         " COUNT(playlistEntries.songId) AS songCount,"
                         " SUM(songs.length) AS duration "
                         "FROM playlists "
                         " LEFT JOIN playlistEntries ON playlists.id=playlistEntries.playlistId "
                         " LEFT JOIN songs ON playlistEntries.songId=songs.id "
                         "GROUP BY playlists.id";
    GroupDataModel * model = new GroupDataModel(QStringList() << "name");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        SqlReader reader(sqlQuery);
        int id;
        QString name;
        int songCount;
        double totalDuration;
        reader >> id >> name >> songCount >> totalDuration;
        QObject *value = new NamedPlaylist(id,
                                           name,
                                           songCount,
                                           totalDuration,
                                           model);
        model->insert(value);
    }
    return model;
}

GroupDataModel* Catalog::findAlbums(QString const& searchTerm) {
    const char * query = "SELECT"
                         " albums.id AS id,"
                         " artists.name AS artistName,"
                         " albums.name AS albumName,"
                         " COUNT(albumEntries.songId) AS songCount, "
                         " SUM(songs.length) AS duration "
                         "FROM albums "
                         " INNER JOIN artists ON albums.artistId=artists.id"
                         " INNER JOIN albumEntries ON albums.id=albumEntries.albumId"
                         " INNER JOIN songs ON albumEntries.songId=songs.id ";
    QString whereClause;
    if(searchTerm.length() > 0) {
        QString expr = QString(searchTerm).replace("'", "''")
                                          .replace("\\", "\\\\")
                                          .replace("%", "\\%")
                                          .replace("_", "\\_");
        whereClause += QString(" WHERE (albums.name LIKE '%%%1%%' ESCAPE '\\') OR"
                               "       (albums.name LIKE '%%%1%%' ESCAPE '\\') ").arg(expr);
    }

    QString fullSql(query);
    fullSql += whereClause;
    fullSql += " GROUP BY albums.id";

    GroupDataModel * model = new GroupDataModel(QStringList() << "artistName" << "albumName");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(fullSql);
    while(sqlQuery.next()) {
        SqlReader reader(sqlQuery);
        int id;
        QString artistName;
        QString albumName;
        int songCount;
        double totalDuration;
        reader >> id >> artistName >> albumName >> songCount >> totalDuration;
        QObject *value = new Album(id,
                                   artistName,
                                   albumName,
                                   songCount,
                                   totalDuration,
                                   model);
        model->insert(value);
    }
#ifdef VERBOSE_LOGGING
    qDebug() << "Albums=" << model->size();
#endif
    return model;
}

ArrayDataModel* Catalog::findSongsByFormatId(int formatId, int limit) {
    return selectSongBasicInfo(SELECT_FROM_SONGS,
                               QString("WHERE format=%1").arg(formatId),
                               "ORDER BY "
                               "favourited DESC, "
                               "score DESC, "
                               "downloads DESC, "
                               "playCount DESC, "
                               "lastPlayed DESC, "
                               "fileName ASC ",
                               limit);
}

ArrayDataModel* Catalog::findSongsByGenreId(int genreId, int limit) {
    return selectSongBasicInfo(SELECT_FROM_SONGS,
                               QString("WHERE genre=%1").arg(genreId),
                               "ORDER BY "
                               "favourited DESC, "
                               "score DESC, "
                               "downloads DESC, "
                               "playCount DESC, "
                               "lastPlayed DESC, "
                               "fileName ASC ",
                               limit);
}

ArrayDataModel* Catalog::findSongsByArtistId(int artistId, int limit) {
    return selectSongBasicInfo(SELECT_FROM_SONGS,
                               QString("WHERE artist=%1").arg(artistId),
                               "ORDER BY "
                               "favourited DESC, "
                               "score DESC, "
                               "downloads DESC, "
                               "playCount DESC, "
                               "lastPlayed DESC, "
                               "fileName ASC ",
                               limit);
}

ArrayDataModel* Catalog::findSongsByPlaylistId(int playlistId, int limit) {
    QString selectClause(SELECT_FROM_SONGS);
    selectClause += " INNER JOIN playlistEntries ON songs.id=playlistEntries.songId ";
    return selectSongBasicInfo(selectClause,
                               QString("WHERE playlistEntries.playlistId=%1").arg(playlistId),
                               "ORDER BY "
                               "playlistEntries.songOrder ASC, "
                               "songs.playCount DESC, "
                               "songs.lastPlayed DESC, "
                               "songs.fileName ASC ",
                               limit);
}

ArrayDataModel* Catalog::findSongsByAlbumId(int albumId, int limit) {
    QString selectClause(SELECT_FROM_SONGS);
    selectClause += " INNER JOIN albumEntries ON songs.id=albumEntries.songId ";
    return selectSongBasicInfo(selectClause,
                               QString("WHERE albumEntries.albumId=%1").arg(albumId),
                               "ORDER BY "
                               "albumEntries.trackNumber ASC, "
                               "songs.fileName ASC, "
                               "songs.playCount DESC, "
                               "songs.lastPlayed DESC ",
                               limit);
}

QString Catalog::resolveFileNameById(int id) {
    QString query = "SELECT fileName FROM songs WHERE id=?";
    QVariantList params;
    params << id;
    QVariantList data = m_dataAccess->execute(query, params).value<QVariantList>();
    if(data.size() == 1)
    {
        QVariant const& first = data.first();
        QMap<QString, QVariant> map = first.toMap();
        return map["fileName"].toString();
    }
    else
    {
        return "";
    }
}

int Catalog::resolveModuleIdByFileName(QString const& fileName) {
    QString query = "SELECT id FROM songs WHERE fileName=?";
    QVariantList params;
    params << fileName;
    QVariantList data = m_dataAccess->execute(query, params).value<QVariantList>();
    if(data.size() == 1)
    {
        QVariant const& first = data.first();
        QMap<QString, QVariant> map = first.toMap();
        return map["id"].toInt();
    }
    else
    {
        return 0;
    }
}

SongExtendedInfo* Catalog::resolveModuleById(int id, QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    return selectSongInfo(QString("WHERE songs.id=%1").arg(id), parentObject);
}

SongExtendedInfo* Catalog::resolveModuleByFileName(QString const& fileName, QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    QString escaped(fileName);
    escaped.replace("'", "''");
    return selectSongInfo(QString("WHERE songs.fileName='%1'").arg(escaped), parentObject);
}

SongExtendedInfo* Catalog::readSongInfo(QSqlQuery &sqlQuery, QObject *parent) {
    int id;
    QString fileName;
    QString title;
    int formatId;
    int downloads;
    int favourited;
    int score;
    int size;
    int length;
    int playCount;
    int lastPlayed;
    int myFavourite;
    QString format;
    QString tracker;
    QString genre;
    int artistId;
    int genreId;
    int trackerId;
    QString artist;
    int patterns;
    int orders;
    int instruments;
    int samples;
    int channels;
    SqlReader reader(sqlQuery);
    reader  >> id
            >> fileName
            >> title
            >> formatId
            >> downloads
            >> favourited
            >> score
            >> size
            >> length
            >> playCount
            >> lastPlayed
            >> myFavourite
            >> format
            >> tracker
            >> genre
            >> artistId
            >> genreId
            >> trackerId
            >> artist
            >> patterns
            >> orders
            >> instruments
            >> samples
            >> channels;
    return new SongExtendedInfo(
                        id,
                        fileName,
                        title,
                        downloads,
                        favourited,
                        score,
                        size,
                        length,
                        playCount,
                        lastPlayed,
                        myFavourite,
                        format,
                        tracker,
                        genre,
                        artistId,
                        genreId,
                        trackerId,
                        artist,
                        patterns,
                        orders,
                        instruments,
                        samples,
                        channels,
                        parent);
}

SongBasicInfo* Catalog::readSongBasicInfo(QSqlQuery &sqlQuery, QObject *parent) {
    SqlReader reader(sqlQuery);
    int id;
    QString fileName;
    QString title;
    int format;
    int downloads;
    int favourited;
    int score;
    int size;
    int length;
    int playCount;
    int lastPlayed;
    int myFavourite;
    reader >> id >> fileName >> title >> format >> downloads >> favourited >> score >> size >> length >> playCount >> lastPlayed >> myFavourite;
    return new SongBasicInfo(id,
                             fileName,
                             title,
                             downloads,
                             favourited,
                             score,
                             size,
                             length,
                             playCount,
                             lastPlayed,
                             myFavourite,
                             parent);
}

SongExtendedInfo* Catalog::selectSongInfo(QString const& whereClause, QObject *parent) {
    QString query =
                "SELECT"
                " songs.id AS id,"
                " songs.fileName AS fileName,"
                " songs.title as title,"
                " songs.format AS formatId, "
                " songs.downloads AS downloads, "
                " songs.favourited AS favourited, "
                " songs.score AS score, "
                " songs.size AS size, "
                " songs.length AS length, "
                " songs.playCount AS playCount, "
                " songs.lastPlayed AS lastPlayed, "
                " songs.myFavourite AS myFavourite, "
                " formats.description AS format, "
                " trackers.name AS tracker, "
                " genres.name AS genre, "
                " songs.artist AS artistId, "
                " songs.genre AS genreId, "
                " songs.tracker AS trackerId, "
                " artists.name AS artist, "
                " songs.patterns AS patterns, "
                " songs.orders AS orders, "
                " songs.instruments AS instruments, "
                " songs.samples AS samples, "
                " songs.channels AS channels "
                "FROM songs"
                " INNER JOIN trackers ON trackers.id=songs.tracker "
                " INNER JOIN formats ON formats.id=songs.format "
                " INNER JOIN artists ON artists.id=songs.artist "
                " INNER JOIN genres ON genres.id=songs.genre ";
    if(whereClause.length() > 0) {
        query += " ";
        query += whereClause;
    }
    SongExtendedInfo * song = NULL;
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    if(sqlQuery.next()) {
        song = readSongInfo(sqlQuery, parent);
    }
    return song;
}

ArrayDataModel* Catalog::selectSongBasicInfo(QString const& selectClause,
                                             QString const& whereClause,
                                             QString const& orderByClause,
                                             int limit) {
    QString query(selectClause);
    if(whereClause.length() > 0) {
        query += " ";
        query += whereClause;
    }
    if(orderByClause.length() > 0) {
        query += " ";
        query += orderByClause;
    }
    if(limit > 0) {
        query += QString(" LIMIT %1").arg(limit + 1);
    }
    ArrayDataModel * model = new ArrayDataModel();
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        QObject* value = readSongBasicInfo(sqlQuery, model);
        QVariant v = QVariant::fromValue(value);
        model->append(v);
    }
    return model;
}

ArrayDataModel* Catalog::searchSongs(QString const& searchTerm, int limit) {
    QString selectClause = QString(SELECT_FROM_SONGS);
    QString whereClause;
    if(searchTerm.length() > 0) {
        Analytics::getInstance()->search(searchTerm);

        QString expr = QString(searchTerm).replace("'", "''")
                                          .replace("\\", "\\\\")
                                          .replace("%", "\\%")
                                          .replace("_", "\\_");
        whereClause += QString(" WHERE (fileName LIKE '%%%1%%' ESCAPE '\\'"
                               " OR title LIKE '%%%1%%' ESCAPE '\\') ").arg(expr);
    }

    QString orderClause(" ORDER BY favourited DESC, score DESC, downloads DESC ");
    QString limitClause;
    if(limit > 0) {
        limitClause = QString(" LIMIT %1").arg(limit + 1);
    }

    QString const query = selectClause + whereClause + orderClause + limitClause;
#ifdef VERBOSE_LOGGING
    qDebug() << "Search query:" << query;
#endif
    ArrayDataModel * model = new ArrayDataModel();
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery(query, db);
    while(sqlQuery.next()) {
        QObject* value = readSongBasicInfo(sqlQuery, model);
        QVariant v = QVariant::fromValue(value);
        model->append(v);
    }
    return model;
}

ArrayDataModel* Catalog::findMostDownloadedSongs(int limit) {
    return selectSongBasicInfo(SELECT_FROM_SONGS,
                               "WHERE downloads>0",
                               "ORDER BY downloads DESC",
                               limit);
}

ArrayDataModel* Catalog::findMostFavouritedSongs(int limit) {
    return selectSongBasicInfo(SELECT_FROM_SONGS,
                               "WHERE favourited>0",
                               "ORDER BY favourited DESC, downloads DESC, score DESC",
                               limit);
}

ArrayDataModel* Catalog::findMyLocalSongs(int limit) {
    return selectSongBasicInfo(SELECT_FROM_SONGS,
                               "WHERE id<0",
                               "ORDER BY fileName",
                               limit);
}

ArrayDataModel* Catalog::findMostScoredSongs(int limit) {
    return selectSongBasicInfo(SELECT_FROM_SONGS,
                               "WHERE score>0",
                               "ORDER BY score DESC, downloads DESC, favourited DESC",
                               limit);
}

ArrayDataModel* Catalog::findRecentlyPlayedSongs(int limit) {
    return selectSongBasicInfo(SELECT_FROM_SONGS,
                               "WHERE lastPlayed>0",
                               "ORDER BY lastPlayed DESC",
                               limit);
}

ArrayDataModel* Catalog::findMyFavouriteSongs(int limit) {
    return selectSongBasicInfo(SELECT_FROM_SONGS,
                               "WHERE myFavourite>0",
                               "ORDER BY playCount DESC, lastPlayed DESC",
                               limit);
}

ArrayDataModel* Catalog::findMostPlayedSongs(int limit) {
    return selectSongBasicInfo(SELECT_FROM_SONGS,
                               "WHERE playCount>0",
                               "ORDER BY playCount DESC, lastPlayed DESC",
                               limit);
}

void Catalog::addFavourite(QVariant value) {
    SongBasicInfo * info = songBasicInfo(value);
    if(info != 0) {
        info->setMyFavourite(info->myFavourite() + 1);
        QString query = "UPDATE songs SET myFavourite=? WHERE id=?";
        QVariantList params;
        params << info->myFavourite() << info->id();
        m_dataAccess->execute(query, params);

        Analytics::getInstance()->addFavourite(info->id(), FileUtils::fileNameOnly(info->fileName()));
    } else {
        qDebug() << "!!! Catalog::addFavourite: invalid value=" << value;
    }
}

void Catalog::removeFavourite(QVariant value) {
    SongBasicInfo * info = songBasicInfo(value);
    if(info != 0) {
        info->setMyFavourite(0);
        QString query = "UPDATE songs SET myFavourite=? WHERE id=?";
        QVariantList params;
        params << info->myFavourite() << info->id();
        m_dataAccess->execute(query, params);

        Analytics::getInstance()->removeFavourite(info->id(), FileUtils::fileNameOnly(info->fileName()));
    } else {
        qDebug() << "!!! Catalog::removeFavourite: invalid value=" << value;
    }
}

void Catalog::play(QVariant value) {
    SongBasicInfo * info = songBasicInfo(value);
    if(info != 0) {
        uint now = QDateTime::currentDateTime().toTime_t();
        info->setPlayCount(info->playCount() + 1);
        info->setLastPlayed(now);
        QString query = "UPDATE songs SET playCount=?, lastPlayed=? WHERE id=?";
        QVariantList params;
        params << info->playCount() << info->lastPlayed() << info->id();
        m_dataAccess->execute(query, params);
        Analytics::getInstance()->play(info->id(), FileUtils::fileNameOnly(info->fileName()));
    } else {
        qDebug() << "!!! Catalog::play: invalid value=" << value;
    }
}

void Catalog::resetPlayCounts() {
    QString query = QString("UPDATE songs "
                            "SET playCount=0,"
                            "    lastPlayed=0 "
                            "WHERE playCount>0 OR lastPlayed>0");
    m_dataAccess->execute(query);

    Analytics::getInstance()->resetPlayCounts();
}

void Catalog::resetMyFavourites() {
    QString query = QString("UPDATE songs "
                            "SET myFavourite=0 "
                            "WHERE myFavourite>0");
    m_dataAccess->execute(query);

    Analytics::getInstance()->resetMyFavourites();
}

void Catalog::deleteSong(int songId) {
    m_dataAccess->execute(QString("DELETE FROM songs WHERE id=%1").arg(songId));
}

void Catalog::houseKeep() {
    QString query = "DELETE FROM playlistEntries WHERE songId NOT IN (SELECT id from songs)";
    m_dataAccess->execute(query);

    query = "DELETE FROM albumEntries WHERE songId NOT IN (SELECT id from songs)";
    m_dataAccess->execute(query);

    query = "DELETE FROM albums WHERE id IN (SELECT id FROM albums WHERE id NOT IN (SELECT albumId FROM albumEntries))";
    m_dataAccess->execute(query);

    query = "DELETE FROM artists WHERE id IN (SELECT id FROM artists WHERE id NOT IN (SELECT artist FROM songs))";
    m_dataAccess->execute(query);
}

void Catalog::addPersonalSong(SongExtendedInfo const& info) {
    QString query = QString("INSERT INTO songs"
                            " (id,"
                            " fileName,"
                            " title,"
                            " format,"
                            " downloads,"
                            " favourited,"
                            " score,"
                            " size,"
                            " length,"
                            " playCount,"
                            " lastPlayed,"
                            " myFavourite,"
                            " tracker,"
                            " genre,"
                            " artist,"
                            " patterns,"
                            " orders,"
                            " instruments,"
                            " samples,"
                            " channels) "
                            "VALUES "
                            "(?,?,?,?,?,?,?,?,?,?,"
                            "?,?,?,?,?,?,?,?,?,?)");
    QVariantList params;
    params  << info.id()
            << info.fileName()
            << info.title()
            << info.formatId()
            << info.downloads()
            << info.favourited()
            << info.score()
            << info.fileSize()
            << info.songLength()
            << info.playCount()
            << info.lastPlayed()
            << info.myFavourite()
            << info.trackerId()
            << info.genreId()
            << info.artistId()
            << info.patterns()
            << info.orders()
            << info.instruments()
            << info.samples()
            << info.channels();
    m_dataAccess->execute(query, params);
}

void Catalog::addSongToAlbum(int albumId, int songId, int trackNumber) {
    QString query = "INSERT INTO albumEntries (albumId, songId, trackNumber) VALUES (?,?,?)";
    QVariantList params;
    params << albumId << songId << trackNumber;
    m_dataAccess->execute(query, params);
}

int Catalog::createGenre(QString const& name) {
    int primaryKey = 0;
    QString query = "SELECT id FROM genres WHERE name=?";
    QVariantList params;
    params << name.trimmed();
    QVariant result = m_dataAccess->execute(query, params);
    QVariantList list = result.value<QVariantList>();
    if(list.size() >= 1) {
        primaryKey = list[0].value<QVariantMap>()["id"].value<int>();
    } else {
        query = "SELECT MIN(id)-1 FROM genres";
        QSqlDatabase db = m_dataAccess->connection();
        QSqlQuery sqlQuery = db.exec(query);
        if(sqlQuery.next()) {
            primaryKey = sqlQuery.value(0).toInt();
        } else {
            return primaryKey; // error
        }

        query = "INSERT INTO genres (id, name) VALUES (?,?)";
        QVariantList params;
        params << primaryKey << name.trimmed();
        m_dataAccess->execute(query, params);
    }
    return primaryKey;
}

int Catalog::createArtist(QString const& name) {
    int primaryKey = 0;
    QString query = "SELECT id FROM artists WHERE name=?";
    QVariantList params;
    params << name.trimmed();
    QVariant result = m_dataAccess->execute(query, params);
    QVariantList list = result.value<QVariantList>();
    if(list.size() >= 1) {
        primaryKey = list[0].value<QVariantMap>()["id"].value<int>();
    } else {
        query = "SELECT MIN(id)-1 FROM artists";
        QSqlDatabase db = m_dataAccess->connection();
        QSqlQuery sqlQuery = db.exec(query);
        if(sqlQuery.next()) {
            primaryKey = sqlQuery.value(0).toInt();
        } else {
            return primaryKey; // error
        }

        query = "INSERT INTO artists (id, name, score, downloads, rating) VALUES (?,?,0,0,0)";
        QVariantList params;
        params << primaryKey << name.trimmed();
        m_dataAccess->execute(query, params);
    }
    return primaryKey;
}

int Catalog::createPlaylist(QString const& name) {
    int primaryKey = 0;
    QString query = "SELECT COALESCE(MAX(id) , 0) + 1 FROM playlists";
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    if(sqlQuery.next()) {
        primaryKey = sqlQuery.value(0).toInt();
    } else {
        return primaryKey; // error
    }
#ifdef VERBOSE_LOGGING
    qDebug().nospace() << "Creating playlist (" << primaryKey << "," << name.trimmed() << ")";
    qDebug().space();
#endif
    query = "INSERT INTO playlists (id, name) VALUES (?,?)";
    QVariantList params;
    params << primaryKey << name.trimmed();
    m_dataAccess->execute(query, params);

    Analytics::getInstance()->createPlaylist(name.trimmed());

    return primaryKey;
}

void Catalog::deletePlaylist(int playlistId) {
    QVariantList params;
    params << playlistId;

    QString query = "DELETE FROM playlistEntries WHERE playlistId=?";
    m_dataAccess->execute(query, params);

    query = "DELETE FROM playlists WHERE id=?";
    m_dataAccess->execute(query, params);
}

void Catalog::deleteAllPlaylists() {
    QString query = "DELETE FROM playlistEntries";
    m_dataAccess->execute(query);

    query = "DELETE FROM playlists";
    m_dataAccess->execute(query);

    Analytics::getInstance()->deleteAllPlaylists();
}

void Catalog::appendToPlaylist(int playlistId, int songId) {
    QString query = QString("SELECT COALESCE(MAX(songOrder),0)+1 FROM playlistEntries WHERE playlistId=%1").arg(playlistId);
    query = QString("INSERT INTO playlistEntries (playlistId, songId, songOrder) VALUES (?,?,(%1))").arg(query);
    QVariantList params;
    params << playlistId << songId;
    m_dataAccess->execute(query, params);
}

void Catalog::deleteSongFromPlaylist(int playlistId, int songId) {
    QString query = "DELETE FROM playlistEntries WHERE playlistId=? AND songId=?";
    QVariantList params;
    params << playlistId << songId;
    m_dataAccess->execute(query, params);
}

QVariant Catalog::getPlaylistSongs(int playlistId) {
    QVariantList result;
    QString query = QString("SELECT songId FROM playlistEntries WHERE playlistId=%1 ORDER BY songOrder ASC").arg(playlistId);
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int songId = sqlQuery.value(0).toInt();
        result << QVariant::fromValue(songId);
    }
    return QVariant::fromValue(result);
}

QVariant Catalog::getAlbumSongs(int albumId) {
    QVariantList result;
    QString query = QString("SELECT songId "
                            "FROM albumEntries "
                            "INNER JOIN songs ON songs.id = albumEntries.songId "
                            "WHERE albumEntries.albumId=%1 "
                            "ORDER BY albumEntries.trackNumber ASC, songs.fileName ASC").arg(albumId);
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int songId = sqlQuery.value(0).toInt();
        result << QVariant::fromValue(songId);
    }
    return QVariant::fromValue(result);
}

int Catalog::createAlbum(QString const& artistName, QString const& albumName) {
    int primaryKey = 0;
    int artistId = createArtist(artistName.trimmed());
    QString query = "SELECT id FROM albums WHERE artistId=? AND name=?";
    QVariantList params;
    params << artistId << albumName.trimmed();
    QVariant result = m_dataAccess->execute(query, params);
    QVariantList list = result.value<QVariantList>();
    if(list.size() >= 1) {
        primaryKey = list[0].value<QVariantMap>()["id"].value<int>();
    } else {
        query = "SELECT COALESCE(MAX(id) , 0) + 1 FROM albums";
        QSqlDatabase db = m_dataAccess->connection();
        QSqlQuery sqlQuery = db.exec(query);
        if(sqlQuery.next()) {
            primaryKey = sqlQuery.value(0).toInt();
        } else {
            return primaryKey; // error
        }

        query = "INSERT INTO albums (id, artistId, name) VALUES (?,?,?)";
        QVariantList params;
        params << primaryKey << artistId << albumName.trimmed();
        m_dataAccess->execute(query, params);

        Analytics::getInstance()->createAlbum(artistName.trimmed(), albumName.trimmed());
    }
    return primaryKey;
}

