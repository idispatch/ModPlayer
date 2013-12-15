#include "FileUtils.hpp"
#include "Catalog.hpp"
#include "Analytics.hpp"
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <bb/data/SqlDataAccess>
#include <bb/data/DataSource>

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

struct PersonalData {
    int id;
    int playCount;
    int lastPlayed;
    int myFavourite;
};

typedef QVector<PersonalData> MigrationData;

Catalog::Catalog(QObject * parent)
    : QThread(parent),
      m_dataAccess(NULL) {
    initCatalog();
}

void Catalog::initCatalog() {
    copyCatalogToDataFolder();
    m_dataAccess = new SqlDataAccess(catalogPath(), "catalog", this);
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

void Catalog::copyCatalogToDataFolder() {
    QFile catalogFile(catalogPath());
    if(!catalogFile.exists())
    {
        qDebug() << "Personal catalog does not exist at" << catalogFile.fileName();

        QString appFolder(QDir::homePath());
        appFolder.chop(4);
        QString originalFileName = appFolder + "app/native/assets/catalog.sqlite";
        QFile originalFile(originalFileName);

        if(originalFile.exists())
        {
            qDebug() << "Original catalog is at" << originalFile.fileName();

            if(!originalFile.copy(catalogFile.fileName()))
            {
                qDebug() << "Failed to copy catalog to path: " << catalogFile.fileName();
            }
        }
        else
        {
            qDebug() << "Failed to copy catalog - file does not exists";
        }
    }
    else
    {
        // catalog exists
        MigrationData data;
        {
            int version = 0;
            SqlDataAccess dataAccess(catalogPath(), "catalog");
            {
                QSqlDatabase db = dataAccess.connection();
                QSqlQuery sqlQuery = db.exec("PRAGMA user_version");
                if(sqlQuery.next()) {
                    version = sqlQuery.value(0).toInt();
                }
            }

            if(version == 2)
            {
                qDebug() << "Database migration not required, version" << version;
                QSqlDatabase::removeDatabase("catalog");
                return;
            }
            else
            {
                qDebug() << "Starting database migration from version" << version;
                const char * query =    "SELECT"
                                        " id,"
                                        " playCount,"
                                        " lastPlayed,"
                                        " myFavourite "
                                        "FROM songs "
                                        "WHERE playCount>0 OR lastPlayed>0 OR myFavourite>0";
                QSqlDatabase db = dataAccess.connection();
                QSqlQuery sqlQuery = db.exec(query);
                while(sqlQuery.next()) {
                    PersonalData song;
                    int column = 0;
                    song.id          = sqlQuery.value(column++).toInt();
                    song.playCount   = sqlQuery.value(column++).toInt();
                    song.lastPlayed  = sqlQuery.value(column++).toInt();
                    song.myFavourite = sqlQuery.value(column++).toInt();
                    data.push_back(song);
                }
                qDebug() << "Migrating" << data.size() << "songs";
            }
        }

        QSqlDatabase::removeDatabase("catalog");

        qDebug() << "Removing old database";
        catalogFile.remove();

        qDebug() << "Removed old database";
        copyCatalogToDataFolder();

        qDebug() << "About to update" << data.size() << "songs";
        if(!data.empty())
        {
            SqlDataAccess dataAccess(catalogPath(), "catalog");
            for(int i = 0; i<data.size(); ++i) {
                QString query =
                    "UPDATE songs SET playCount=?, lastPlayed=?, myFavourite=? WHERE id=?";
                QVariantList params;
                params << data[i].playCount
                       << data[i].lastPlayed
                       << data[i].myFavourite
                       << data[i].id;
                qDebug() << "Updating song:" << data[i].id;
                dataAccess.execute(query, params);
            }

            QSqlDatabase::removeDatabase("catalog");
        } else {
            qDebug() << "No song updates required";
        }

        qDebug() << "Migration completed successfully";
    }
}

int Catalog::asyncCommandSubmit(Command * command) {
    QMutexLocker locker(&m_mutex);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::asyncCommand(Command::CommandType commandType) {
    return asyncCommandSubmit(new Command(commandType));
}

int Catalog::asyncFindCommand(Command::CommandType commandType, int id, int limit) {
    return asyncCommandSubmit(new FindCommand(commandType, id, limit));
}

int Catalog::asyncSearchCommand(QString const& query, int limit) {
    return asyncCommandSubmit(new SearchCommand(query, limit));
}

int Catalog::songCount() {
    const char * query = "SELECT count(id) FROM songs WHERE id > 0";
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    if(sqlQuery.next()) {
        return sqlQuery.value(0).toInt();
    }
    return 0;
}

int Catalog::songCountAsync() {
    return asyncCommand(Command::SongCount);
}

int Catalog::personalSongCount() {
    const char * query = "SELECT count(id) FROM songs WHERE id < 0";
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    if(sqlQuery.next()) {
        return sqlQuery.value(0).toInt();
    }
    return 0;
}

int Catalog::personalSongCountAsync() {
    return asyncCommand(Command::PersonalSongCount);
}

int Catalog::findFormatsAsync() {
    return asyncCommand(Command::FormatsList);
}

int Catalog::findGenresAsync() {
    return asyncCommand(Command::GenresList);
}

int Catalog::findArtistsAsync() {
    return asyncCommand(Command::ArtistsList);
}

int Catalog::findPlaylistsAsync() {
    return asyncCommand(Command::Playlists);
}

int Catalog::findSongsByFormatIdAsync(int formatId, int limit) {
    return asyncFindCommand(Command::SongsByFormatList, formatId, limit);
}

int Catalog::findSongsByGenreIdAsync(int genreId, int limit) {
    return asyncFindCommand(Command::SongsByGenreList, genreId, limit);
}

int Catalog::findSongsByArtistIdAsync(int artistId, int limit) {
    return asyncFindCommand(Command::SongsByArtistList, artistId, limit);
}

int Catalog::findSongsByPlaylistIdAsync(int playlistId, int limit) {
    return asyncFindCommand(Command::SongsByPlaylist, playlistId, limit);
}

int Catalog::findMostDownloadedSongsAsync(int limit) {
    return asyncFindCommand(Command::MostDownloadedSongs, 0, limit);
}

int Catalog::findMostFavouritedSongsAsync(int limit) {
    return asyncFindCommand(Command::MostFavouritedSongs, 0, limit);
}

int Catalog::findMostScoredSongsAsync(int limit) {
    return asyncFindCommand(Command::MostScoredSongs, 0, limit);
}

int Catalog::findRecentlyPlayedSongsAsync(int limit) {
    return asyncFindCommand(Command::RecentlyPlayedSongs, 0, limit);
}

int Catalog::findMyFavouriteSongsAsync(int limit) {
    return asyncFindCommand(Command::MyFavouriteSongs, 0, limit);
}

int Catalog::findMyLocalSongsAsync(int limit) {
    return asyncFindCommand(Command::MyLocalSongs, 0, limit);
}

int Catalog::findMostPlayedSongsAsync(int limit) {
    return asyncFindCommand(Command::MostPlayedSongs, 0, limit);
}

int Catalog::searchSongsAsync(QString const& searchTerm, int limit) {
    return asyncSearchCommand(searchTerm, limit);
}

QString Catalog::catalogPath() const {
    return QDir::homePath() + "/catalog.sqlite";
}

ArrayDataModel* Catalog::findFormats() {
    const char * query =
            "SELECT"
            " formats.id AS id, "
            " formats.name AS name, "
            " formats.description AS description, "
            " count(formats.id) AS count "
            "FROM formats "
            "INNER JOIN songs "
            " ON songs.format=formats.id "
            "GROUP BY formats.id";
    ArrayDataModel * model = new ArrayDataModel();
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int column   = 0;
        int id       = sqlQuery.value(column++).toInt();
        QString name = sqlQuery.value(column++).toString();
        QString desc = sqlQuery.value(column++).toString();
        int count    = sqlQuery.value(column++).toInt();
        QObject* value = new SongFormat(id,
                                        name,
                                        desc,
                                        count,
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
            " COUNT(songs.id) AS count "
            "FROM genres "
            "LEFT JOIN songs "
            " ON songs.genre=genres.id "
            "GROUP BY genres.id";
    GroupDataModel * model = new GroupDataModel(QStringList() << "name");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int column   = 0;
        int id       = sqlQuery.value(column++).toInt();
        QString name = sqlQuery.value(column++).toString();
        int count    = sqlQuery.value(column++).toInt();
        QObject* value = new SongGenre(id, name, count, model);
        model->insert(value);
    }
    return model;
}

GroupDataModel* Catalog::findArtists() {
    const char * query =
            "SELECT"
            " artists.id,"
            " artists.name,"
            " artists.score,"
            " artists.downloads,"
            " artists.rating,"
            " COUNT(songs.id) AS count "
            "FROM artists "
            "LEFT JOIN songs "
            " ON songs.artist=artists.id "
            "GROUP BY artists.id";
    GroupDataModel * model = new GroupDataModel(QStringList() << "name");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int column    = 0;
        int id        = sqlQuery.value(column++).toInt();
        QString name  = sqlQuery.value(column++).toString();
        int score     = sqlQuery.value(column++).toInt();
        int downloads = sqlQuery.value(column++).toInt();
        int rating    = sqlQuery.value(column++).toInt();
        int count     = sqlQuery.value(column++).toInt();
        QObject *value = new Artist(id,
                                    name,
                                    score,
                                    downloads,
                                    rating,
                                    count,
                                    model);
        model->insert(value);
    }
    return model;
}

GroupDataModel* Catalog::findPlaylists() {
    const char * query =
            "SELECT"
            " playlists.id,"
            " playlists.name, "
            " COUNT(playlistEntries.songId) AS songCount "
            "FROM playlists "
            "LEFT JOIN playlistEntries "
            " ON playlists.id=playlistEntries.playlistId "
            "GROUP BY playlists.id";
    GroupDataModel * model = new GroupDataModel(QStringList() << "name");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int column    = 0;
        int id        = sqlQuery.value(column++).toInt();
        QString name  = sqlQuery.value(column++).toString();
        int count     = sqlQuery.value(column++).toInt();
        QObject *value = new NamedPlaylist(id,
                                           name,
                                           count,
                                           model);
        model->insert(value);
    }
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
    return selectSongInfo(QString("WHERE songs.id=%1").arg(id),
                          parentObject);
}

SongExtendedInfo* Catalog::resolveModuleByFileName(QString const& fileName, QVariant parent) {
    //TODO: fix SQL injection
    QObject * parentObject = parent.value<QObject*>();
    return selectSongInfo(QString("WHERE songs.fileName='%1'").arg(fileName),
                          parentObject);
}

SongExtendedInfo* Catalog::readSongInfo(QSqlQuery &sqlQuery, QObject *parent) {
    int column       = 0;
    int id           = sqlQuery.value(column++).toInt();
    QString fileName = sqlQuery.value(column++).toString();
    QString title    = sqlQuery.value(column++).toString();
    int formatId     = sqlQuery.value(column++).toInt();
    int downloads    = sqlQuery.value(column++).toInt();
    int favourited   = sqlQuery.value(column++).toInt();
    int score        = sqlQuery.value(column++).toInt();
    int size         = sqlQuery.value(column++).toInt();
    int length       = sqlQuery.value(column++).toInt();
    int playCount    = sqlQuery.value(column++).toInt();
    int lastPlayed   = sqlQuery.value(column++).toInt();
    int myFavourite  = sqlQuery.value(column++).toInt();
    QString format   = sqlQuery.value(column++).toString();
    QString tracker  = sqlQuery.value(column++).toString();
    QString genre    = sqlQuery.value(column++).toString();
    int artistId     = sqlQuery.value(column++).toInt();
    QString artist   = sqlQuery.value(column++).toString();
    int patterns     = sqlQuery.value(column++).toInt();
    int orders       = sqlQuery.value(column++).toInt();
    int instruments  = sqlQuery.value(column++).toInt();
    int samples      = sqlQuery.value(column++).toInt();
    int channels     = sqlQuery.value(column++).toInt();
    return new SongExtendedInfo(
                        id,
                        fileName,
                        title,
                        formatId,
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
                        artist,
                        patterns,
                        orders,
                        instruments,
                        samples,
                        channels,
                        parent);
}

SongBasicInfo* Catalog::readSongBasicInfo(QSqlQuery &sqlQuery, QObject *parent) {
    int column       = 0;
    int id           = sqlQuery.value(column++).toInt();
    QString fileName = sqlQuery.value(column++).toString();
    QString title    = sqlQuery.value(column++).toString();
    int format       = sqlQuery.value(column++).toInt();
    int downloads    = sqlQuery.value(column++).toInt();
    int favourited   = sqlQuery.value(column++).toInt();
    int score        = sqlQuery.value(column++).toInt();
    int size         = sqlQuery.value(column++).toInt();
    int length       = sqlQuery.value(column++).toInt();
    int playCount    = sqlQuery.value(column++).toInt();
    int lastPlayed   = sqlQuery.value(column++).toInt();
    int myFavourite  = sqlQuery.value(column++).toInt();
    return new SongBasicInfo(id,
                             fileName,
                             title,
                             format,
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
    QString query = QString(
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
                " INNER JOIN genres ON genres.id=songs.genre ");
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
#ifdef DEBUG_CATALOG
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

void Catalog::clearPersonalSongs() {
    QString query = "DELETE FROM playlistEntries WHERE songId < 0";
    m_dataAccess->execute(query);
    query = "DELETE FROM songs WHERE id < 0";
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
            << 0  // tracker
            << 0  // genre
            << 0  // artist
            << info.patterns()
            << info.orders()
            << info.instruments()
            << info.samples()
            << info.channels();
    m_dataAccess->execute(query, params);
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
#ifdef DEBUG_CATALOG
    qDebug().nospace() << "Creating playlist (" << primaryKey << "," << name << ")";
    qDebug().space();
#endif
    query = "INSERT INTO playlists (id, name) VALUES (?,?)";
    QVariantList params;
    params << primaryKey << name;
    m_dataAccess->execute(query, params);

    Analytics::getInstance()->createPlaylist(name);

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
    int songOrder = 0;
    QString query = QString("SELECT COALESCE(MAX(songOrder),0)+1 "
                            "FROM playlistEntries "
                            "WHERE playlistId=%1").arg(playlistId);
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    if(sqlQuery.next()) {
        songOrder = sqlQuery.value(0).toInt();
    } else {
        qDebug() << "Failed to find song order in playlist" << playlistId;
        return;
    }
    query = "INSERT INTO playlistEntries (playlistId, songId, songOrder) VALUES (?,?,?)";
    QVariantList params;
    params << playlistId << songId << songOrder;
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
    QString query = QString("SELECT songId FROM playlistEntries WHERE playlistId=%1").arg(playlistId);
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int songId = sqlQuery.value(0).toInt();
        result << QVariant::fromValue(songId);
    }
    return QVariant::fromValue(result);
}

void Catalog::run() {
    bool exitRequested = false;
    while(!exitRequested)
    {
        m_mutex.lock();
        while(m_commandQueue.empty())
        {
            m_cond.wait(&m_mutex);
        }

        std::auto_ptr<Command> command(m_commandQueue.dequeue());
        m_mutex.unlock();

        QVariant result;
        switch(command->command())
        {
        case Command::ExitCommand:
            exitRequested = true;
            m_mutex.lock();
            while(!m_commandQueue.empty())
            {
                delete m_commandQueue.dequeue();
            }
            m_mutex.unlock();
            break;
        case Command::SongCount:
            result = QVariant::fromValue(songCount());
            break;
        case Command::PersonalSongCount:
            result = QVariant::fromValue(personalSongCount());
            break;
        case Command::SearchSongs:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                ArrayDataModel * model = searchSongs(searchCommand->query(), searchCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::FormatsList:
            {
                ArrayDataModel * model = findFormats();
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::GenresList:
            {
                GroupDataModel * model = findGenres();
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::ArtistsList:
            {
                GroupDataModel * model = findArtists();
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::Playlists:
            {
                GroupDataModel * model = findPlaylists();
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByFormatList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findSongsByFormatId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByArtistList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findSongsByArtistId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByGenreList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findSongsByGenreId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByPlaylist:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findSongsByPlaylistId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostDownloadedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMostDownloadedSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostFavouritedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMostFavouritedSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostScoredSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMostScoredSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::RecentlyPlayedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findRecentlyPlayedSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MyFavouriteSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMyFavouriteSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MyLocalSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMyLocalSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostPlayedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMostPlayedSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        default:
            qDebug() << "Unknown command:" << command->command();
            break;
        }

        emit resultReady(command->id(), result);

        QObject* p = result.value<QObject*>();
        if(p != NULL && p->parent() == NULL)
        {
            result.detach();
            delete p;
        }
    }
    QThread::exit(0);
}
