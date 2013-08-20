#include "Catalog.hpp"
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

Catalog::Catalog(QObject * parent)
    : QObject(parent),
      m_dataAccess(NULL) {
    initCatalog();
}

void Catalog::initCatalog() {
    copyCatalogToDataFolder();
    m_dataAccess = new SqlDataAccess(catalogPath(), "catalog", this);
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

QString Catalog::catalogPath() const {
    return QDir::homePath() + "/catalog.sqlite";
}

ArrayDataModel*
Catalog::findFormats(QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
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
    ArrayDataModel * model = new ArrayDataModel(parentObject);
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int column   = 0;
        int id       = sqlQuery.value(column++).toInt();
        QString name = sqlQuery.value(column++).toString();
        QString desc = sqlQuery.value(column++).toString();
        int count    = sqlQuery.value(column++).toInt();
        model->append(QVariant::fromValue(static_cast<QObject*>(new SongFormat(id, name, desc, count, model))));
    }
    qDebug() << "Returning ArrayDataModel with parent " << parentObject;
    return model;
}

GroupDataModel*
Catalog::findGenres(QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    const char * query =
            "SELECT"
            " genres.id,"
            " genres.name,"
            " COUNT(songs.id) AS count "
            "FROM genres "
            "LEFT JOIN songs "
            " ON songs.genre=genres.id "
            "GROUP BY genres.id";
    GroupDataModel * model = new GroupDataModel(QStringList() << "name",
                                                parentObject);
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int column   = 0;
        int id       = sqlQuery.value(column++).toInt();
        QString name = sqlQuery.value(column++).toString();
        int count    = sqlQuery.value(column++).toInt();
        model->insert(static_cast<QObject*>(new SongGenre(id, name, count, model)));
    }
    qDebug() << "Returning GroupDataModel with parent " << parentObject;
    return model;
}

GroupDataModel*
Catalog::findArtists(QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
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
    GroupDataModel * model = new GroupDataModel(QStringList() << "name",
                                                parentObject);
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
        model->insert(static_cast<QObject*>(new Artist(id,
                                                       name,
                                                       score,
                                                       downloads,
                                                       rating,
                                                       count,
                                                       model)));
    }
    qDebug() << "Returning GroupDataModel with parent " << parentObject;
    return model;
}

GroupDataModel*
Catalog::findSongsByFormatId(int formatId, QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    QString query = QString("SELECT"
                            " id, "
                            " fileName, "
                            " title, "
                            " format, "
                            " downloads, "
                            " favourited, "
                            " score, "
                            " size, "
                            " length, "
                            " playCount, "
                            " lastPlayed, "
                            " myFavourite "
                            "FROM songs "
                            "WHERE format=%1").arg(formatId);
    GroupDataModel * model = new GroupDataModel(QStringList() << "fileName"
                                                              << "downloads",
                                                parentObject);
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        model->insert(static_cast<QObject*>(readSongBasicInfo(sqlQuery, model)));
    }

    return model;
}

GroupDataModel*
Catalog::findSongsByGenreId(int genreId, QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    QString query = QString(
            "SELECT "
            " id, "
            " fileName, "
            " title, "
            " format, "
            " downloads, "
            " favourited, "
            " score, "
            " size, "
            " length, "
            " playCount, "
            " lastPlayed, "
            " myFavourite "
            "FROM songs "
            "WHERE genre=%1").arg(genreId);
    GroupDataModel * model = new GroupDataModel(QStringList() << "fileName"
                                                              << "downloads",
                                                parentObject);
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        model->insert(static_cast<QObject*>(readSongBasicInfo(sqlQuery, model)));
    }
    qDebug() << "Returning GroupDataModel with parent " << parentObject;
    return model;
}

GroupDataModel*
Catalog::findSongsByArtistId(int artistId, QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    QString query = QString(
            "SELECT "
            " id, "
            " fileName, "
            " title, "
            " format, "
            " downloads, "
            " favourited, "
            " score, "
            " size, "
            " length, "
            " playCount, "
            " lastPlayed, "
            " myFavourite "
            "FROM songs "
            "WHERE artist=%1").arg(artistId);
    GroupDataModel * model = new GroupDataModel(QStringList() << "fileName"
                                                              << "downloads",
                                                parentObject);
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        model->insert(static_cast<QObject*>(readSongBasicInfo(sqlQuery, model)));
    }
    qDebug() << "Returning GroupDataModel with parent " << parentObject;
    return model;
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
    QString query = QString("SELECT id FROM songs WHERE fileName='%1'").arg(fileName);
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

SongExtendedInfo*
Catalog::resolveModuleById(int id, QVariant parent) {
    QObject * object = parent.value<QObject*>();
    return selectSongInfo(QString("WHERE songs.id=%1").arg(id), object);
}

SongExtendedInfo*
Catalog::resolveModuleByFileName(QString const& fileName, QVariant parent) {
    QObject * object = parent.value<QObject*>();
    return selectSongInfo(QString("WHERE songs.fileName='%1'").arg(fileName), object);
}

SongExtendedInfo*
Catalog::readSongInfo(QSqlQuery &sqlQuery, QObject *parent) {
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
                        artist,
                        patterns,
                        orders,
                        instruments,
                        samples,
                        channels,
                        parent);
}

SongBasicInfo*
Catalog::readSongBasicInfo(QSqlQuery &sqlQuery, QObject *parent) {
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

SongExtendedInfo*
Catalog::selectSongInfo(QString const& whereClause, QObject * parent) {
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

ArrayDataModel*
Catalog::selectSongBasicInfo(QString const& whereClause,
                             QString const& orderByClause,
                             QObject *parent) {
    QString query("SELECT"
                  " id, "
                  " fileName, "
                  " title, "
                  " format, "
                  " downloads, "
                  " favourited, "
                  " score, "
                  " size, "
                  " length, "
                  " playCount, "
                  " lastPlayed, "
                  " myFavourite "
                  "FROM songs ");
    if(whereClause.length() > 0) {
        query += whereClause;
    }
    if(orderByClause.length() > 0) {
        query += orderByClause;
    }
    ArrayDataModel * model = new ArrayDataModel(parent);
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        model->append(QVariant::fromValue(readSongBasicInfo(sqlQuery, model)));
    }
    qDebug() << "Returning ArrayDataModel with parent " << parent;
    return model;
}

ArrayDataModel*
Catalog::findMostDownloadedSongs(QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    return selectSongBasicInfo(" WHERE downloads>0 ",
                               " ORDER BY downloads DESC ",
                               parentObject);
}

ArrayDataModel*
Catalog::findMostFavouritedSongs(QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    return selectSongBasicInfo(" WHERE favourited>0 ",
                               " ORDER BY favourited DESC, downloads DESC, score DESC ",
                               parentObject);
}

ArrayDataModel*
Catalog::findMostScoredSongs(QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    return selectSongBasicInfo(" WHERE score>0 ",
                               " ORDER BY score DESC, downloads DESC, favourited DESC ",
                               parentObject);
}

ArrayDataModel*
Catalog::findRecentlyPlayedSongs(QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    return selectSongBasicInfo(" WHERE lastPlayed>0 ",
                               " ORDER BY lastPlayed DESC ",
                               parentObject);
}

ArrayDataModel*
Catalog::findMyFavouriteSongs(QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    return selectSongBasicInfo(" WHERE myFavourite>0 ",
                               " ORDER BY playCount DESC, lastPlayed DESC ",
                               parentObject);
}

ArrayDataModel*
Catalog::findMostPlayedSongs(QVariant parent) {
    QObject * parentObject = parent.value<QObject*>();
    return selectSongBasicInfo(" WHERE playCount>0 ",
                               " ORDER BY playCount DESC, lastPlayed DESC ",
                               parentObject);
}

void Catalog::addFavourite(QVariant value) {
    SongBasicInfo * info = songBasicInfo(value);
    if(info != 0) {
        qDebug() << "Adding favourite module" << info->fileName();
        info->setMyFavourite(info->myFavourite() + 1);
        QString query = QString("UPDATE songs SET myFavourite=%1 WHERE id=%2").arg(info->myFavourite())
                                                                              .arg(info->id());
        m_dataAccess->execute(query);
    } else {
        qDebug() << "!!! Catalog::addFavourite: invalid value=" << value;
    }
}

void Catalog::removeFavourite(QVariant value) {
    SongBasicInfo * info = songBasicInfo(value);
    if(info != 0) {
        qDebug() << "Removing favourite module" << info->fileName();
        info->setMyFavourite(0);
        QString query = QString("UPDATE songs SET myFavourite=%1 WHERE id=%2").arg(info->myFavourite())
                                                                              .arg(info->id());
        m_dataAccess->execute(query);
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
        qDebug() << "Updating play count="
                 << info->playCount()
                 << ", last played="
                 << info->lastPlayed()
                 << "for song"
                 << info->fileName();
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
}

void Catalog::resetMyFavourites() {
    QString query = QString("UPDATE songs "
                            "SET myFavourite=0 "
                            "WHERE myFavourite>0");
    m_dataAccess->execute(query);
}
