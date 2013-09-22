#include "FileUtils.hpp"
#include "Catalog.hpp"
#include "Analytics.hpp"
#include  <QDebug>

#include  <QtSql/QSqlDatabase>
#include  <QtSql/QSqlQuery>

#include  <bb/data/SqlDataAccess>
#include  <bb/data/DataSource>

using namespace bb::data;
using namespace bb::cascades;

template<>
int InstanceCounter<Catalog>::s_count;
template<>
int InstanceCounter<Catalog>::s_maxCount;

#define SELECT_FROM_SONGS "SELECT id, fileName, title, format, downloads, favourited, score, size, length, playCount, lastPlayed, myFavourite FROM songs "

int Catalog::Command::s_commandCounter = 0;

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
}

int Catalog::songCount() {
    const char * query = "SELECT count(id) FROM songs";
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    if(sqlQuery.next()) {
        return sqlQuery.value(0).toInt();
    }
    return 0;
}

int Catalog::songCountAsync() {
    QMutexLocker locker(&m_mutex);
    Command * command = new Command(Command::SongCount);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findFormatsAsync() {
    QMutexLocker locker(&m_mutex);
    Command * command = new Command(Command::FormatsList);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findGenresAsync() {
    QMutexLocker locker(&m_mutex);
    Command * command = new Command(Command::GenresList);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findArtistsAsync() {
    QMutexLocker locker(&m_mutex);
    Command * command = new Command(Command::ArtistsList);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findSongsByFormatIdAsync(int formatId, int limit) {
    QMutexLocker locker(&m_mutex);
    Command * command = new FindCommand(Command::SongsByFormatList, formatId, limit);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findSongsByGenreIdAsync(int genreId, int limit) {
    QMutexLocker locker(&m_mutex);
    Command * command = new FindCommand(Command::SongsByGenreList, genreId, limit);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findSongsByArtistIdAsync(int artistId, int limit) {
    QMutexLocker locker(&m_mutex);
    Command * command = new FindCommand(Command::SongsByArtistList, artistId, limit);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findMostDownloadedSongsAsync(int limit) {
    QMutexLocker locker(&m_mutex);
    Command * command = new FindCommand(Command::MostDownloadedSongs, 0, limit);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findMostFavouritedSongsAsync(int limit) {
    QMutexLocker locker(&m_mutex);
    Command * command = new FindCommand(Command::MostFavouritedSongs, 0, limit);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findMostScoredSongsAsync(int limit) {
    QMutexLocker locker(&m_mutex);
    Command * command = new FindCommand(Command::MostScoredSongs, 0, limit);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findRecentlyPlayedSongsAsync(int limit) {
    QMutexLocker locker(&m_mutex);
    Command * command = new FindCommand(Command::RecentlyPlayedSongs, 0, limit);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findMyFavouriteSongsAsync(int limit) {
    QMutexLocker locker(&m_mutex);
    Command * command = new FindCommand(Command::MyFavouriteSongs, 0, limit);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::findMostPlayedSongsAsync(int limit) {
    QMutexLocker locker(&m_mutex);
    Command * command = new FindCommand(Command::MostPlayedSongs, 0, limit);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::searchSongsAsync(QString const& searchTerm, int limit) {
    QMutexLocker locker(&m_mutex);
    Command * command = new SearchCommand(searchTerm, limit);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
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

ArrayDataModel* Catalog::findSongsByFormatId(int formatId, int limit) {
    return selectSongBasicInfo(QString("WHERE format=%1").arg(formatId),
                               " ORDER BY "
                               "favourited DESC, "
                               "score DESC, "
                               "downloads DESC, "
                               "playCount DESC, "
                               "lastPlayed DESC, "
                               "fileName ASC ",
                               limit);
}

ArrayDataModel* Catalog::findSongsByGenreId(int genreId, int limit) {
    return selectSongBasicInfo(QString("WHERE genre=%1").arg(genreId),
                               " ORDER BY "
                               "favourited DESC, "
                               "score DESC, "
                               "downloads DESC, "
                               "playCount DESC, "
                               "lastPlayed DESC, "
                               "fileName ASC ",
                               limit);
}

ArrayDataModel* Catalog::findSongsByArtistId(int artistId, int limit) {
    return selectSongBasicInfo(QString("WHERE artist=%1").arg(artistId),
                               " ORDER BY "
                               "favourited DESC, "
                               "score DESC, "
                               "downloads DESC, "
                               "playCount DESC, "
                               "lastPlayed DESC, "
                               "fileName ASC ",
                               limit);
}

QString Catalog::resolveFileNameById(int id) {
#ifdef DEBUG_CATALOG
    qDebug() << "Resolving file name for module id" << id;
#endif
    QString query = QString(
            "SELECT "
            " fileName "
            "FROM songs "
            "WHERE id=%1").arg(id);
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    if(data.size() == 1)
    {
        QVariant const& first = data.first();
        QMap<QString, QVariant> map = first.toMap();
        return map["fileName"].toString();
    }
    else
    {
#ifdef DEBUG_CATALOG
        qDebug() << "File name for module" << id << "cannot be resolved";
#endif
        return "";
    }
}

int Catalog::resolveModuleIdByFileName(QString const& fileName) {
#ifdef DEBUG_CATALOG
    qDebug() << "Resolving module id for file name " << fileName;
#endif
    QString query = QString("SELECT id FROM songs WHERE fileName='%1'").arg(FileUtils::fileNameOnly(fileName));
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    if(data.size() == 1)
    {
        QVariant const& first = data.first();
        QMap<QString, QVariant> map = first.toMap();
        return map["id"].toInt();
    }
    else
    {
#ifdef DEBUG_CATALOG
        qDebug() << "Module id for file name" << fileName << "cannot be resolved";
#endif
        return 0;
    }
}

SongExtendedInfo* Catalog::resolveModuleById(int id, QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    return selectSongInfo(QString("WHERE songs.id=%1").arg(id),
                          parentObject);
}

SongExtendedInfo* Catalog::resolveModuleByFileName(QString const& fileName, QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    return selectSongInfo(QString("WHERE songs.fileName='%1'").arg(FileUtils::fileNameOnly(fileName)),
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

ArrayDataModel* Catalog::selectSongBasicInfo(QString const& whereClause,
                                             QString const& orderByClause,
                                             int limit) {
    QString query(SELECT_FROM_SONGS);
    if(whereClause.length() > 0) {
        query += whereClause;
    }
    if(orderByClause.length() > 0) {
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
    return selectSongBasicInfo(" WHERE downloads>0 ",
                               " ORDER BY downloads DESC ",
                               limit);
}

ArrayDataModel* Catalog::findMostFavouritedSongs(int limit) {
    return selectSongBasicInfo(" WHERE favourited>0 ",
                               " ORDER BY favourited DESC, downloads DESC, score DESC ",
                               limit);
}

ArrayDataModel* Catalog::findMostScoredSongs(int limit) {
    return selectSongBasicInfo(" WHERE score>0 ",
                               " ORDER BY score DESC, downloads DESC, favourited DESC ",
                               limit);
}

ArrayDataModel* Catalog::findRecentlyPlayedSongs(int limit) {
    return selectSongBasicInfo(" WHERE lastPlayed>0 ",
                               " ORDER BY lastPlayed DESC ",
                               limit);
}

ArrayDataModel* Catalog::findMyFavouriteSongs(int limit) {
    return selectSongBasicInfo(" WHERE myFavourite>0 ",
                               " ORDER BY playCount DESC, lastPlayed DESC ",
                               limit);
}

ArrayDataModel* Catalog::findMostPlayedSongs(int limit) {
    return selectSongBasicInfo(" WHERE playCount>0 ",
                               " ORDER BY playCount DESC, lastPlayed DESC ",
                               limit);
}

void Catalog::addFavourite(QVariant value) {
    SongBasicInfo * info = songBasicInfo(value);
    if(info != 0) {
        info->setMyFavourite(info->myFavourite() + 1);
        QString query = QString("UPDATE songs SET myFavourite=%1 WHERE id=%2").arg(info->myFavourite())
                                                                              .arg(info->id());
        m_dataAccess->execute(query);

        Analytics::getInstance()->addFavourite(info->id(), FileUtils::fileNameOnly(info->fileName()));
    } else {
        qDebug() << "!!! Catalog::addFavourite: invalid value=" << value;
    }
}

void Catalog::removeFavourite(QVariant value) {
    SongBasicInfo * info = songBasicInfo(value);
    if(info != 0) {
        info->setMyFavourite(0);
        QString query = QString("UPDATE songs SET myFavourite=%1 WHERE id=%2").arg(info->myFavourite())
                                                                              .arg(info->id());
        m_dataAccess->execute(query);

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
        QString query = QString("UPDATE songs SET playCount=%1, lastPlayed=%2 WHERE id=%3").arg(info->playCount())
                                                                                           .arg(info->lastPlayed())
                                                                                           .arg(info->id());
        m_dataAccess->execute(query);
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

void Catalog::run() {
    bool exitRequested = false;
    while(!exitRequested) {
        m_mutex.lock();
        while(m_commandQueue.empty())
        {
            m_cond.wait(&m_mutex);
        }
        Command * command = m_commandQueue.dequeue();
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
        case Command::SearchSongs:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command);
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
        case Command::SongsByFormatList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command);
                ArrayDataModel * model = findSongsByFormatId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByArtistList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command);
                ArrayDataModel * model = findSongsByArtistId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByGenreList:
        {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command);
                ArrayDataModel * model = findSongsByGenreId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostDownloadedSongs: {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command);
                ArrayDataModel * model = findMostDownloadedSongs(findCommand->limit());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostFavouritedSongs: {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command);
                ArrayDataModel * model = findMostFavouritedSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostScoredSongs: {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command);
                ArrayDataModel * model = findMostScoredSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::RecentlyPlayedSongs: {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command);
                ArrayDataModel * model = findRecentlyPlayedSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MyFavouriteSongs: {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command);
                ArrayDataModel * model = findMyFavouriteSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostPlayedSongs: {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command);
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
        if(p != NULL && p->parent() == NULL) {
            result.detach();
            delete p;
        }

        delete command;
    }
    QThread::exit(0);
}
