#include "Catalog.hpp"
#include "SongFormat.hpp"
#include "SongGenre.hpp"
#include "SongBasicInfo.hpp"
#include "SongInfo.hpp"
#include "Artist.hpp"

#include  <QDebug>

#include  <QtSql/QSqlDatabase>
#include  <QtSql/QSqlQuery>

#include  <bb/data/SqlDataAccess>
#include  <bb/data/DataSource>
#include  <bb/cascades/GroupDataModel>
#include  <bb/cascades/QListDataModel>

using namespace bb::data;
using namespace bb::cascades;

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

DataModel * Catalog::formats() {
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
    typedef QListDataModel<SongFormat*> SongFormatsModel;
    SongFormatsModel * model = new SongFormatsModel();
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int column   = 0;
        int id       = sqlQuery.value(column++).toInt();
        QString name = sqlQuery.value(column++).toString();
        QString desc = sqlQuery.value(column++).toString();
        int count    = sqlQuery.value(column++).toInt();
        model->append(new SongFormat(id, name, desc, count, model));
    }
    return model;
}

DataModel * Catalog::genres() {
    const char * query =
            "SELECT"
            " genres.id,"
            " genres.name,"
            " COUNT(songs.id) AS count "
            "FROM genres "
            "LEFT JOIN songs "
            " ON songs.genre=genres.id "
            "GROUP BY genres.id";

    typedef GroupDataModel SongGenresModel;

    SongGenresModel * model = new SongGenresModel(QStringList() << "name");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        int column   = 0;
        int id       = sqlQuery.value(column++).toInt();
        QString name = sqlQuery.value(column++).toString();
        int count    = sqlQuery.value(column++).toInt();
        model->insert(new SongGenre(id, name, count, model));
    }

    return model;
}

DataModel * Catalog::artists() {
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

    typedef GroupDataModel SongGenresModel;

    SongGenresModel * model = new SongGenresModel(QStringList() << "name");
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
        model->insert(new Artist(id,
                                 name,
                                 score,
                                 downloads,
                                 rating,
                                 count,
                                 model));
    }
    return model;
}

DataModel * Catalog::findSongsByFormatId(int formatId) {
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
    typedef GroupDataModel SongByFormatModel;

    SongByFormatModel * model = new SongByFormatModel(QStringList() << "fileName"
                                                                    << "downloads");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        model->insert(readSongBasicInfo(sqlQuery, model));
    }

    return model;
}

DataModel * Catalog::findSongsByGenreId(int genreId) {
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
    typedef GroupDataModel SongByFormatModel;

    SongByFormatModel * model = new SongByFormatModel(QStringList() << "fileName"
                                                                    << "downloads");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        model->insert(readSongBasicInfo(sqlQuery, model));
    }
    return model;
}

DataModel * Catalog::findSongsByArtistId(int artistId) {
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
    typedef GroupDataModel SongByFormatModel;

    SongByFormatModel * model = new SongByFormatModel(QStringList() << "fileName"
                                                                    << "downloads");
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);

    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        model->insert(readSongBasicInfo(sqlQuery, model));
    }
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

SongInfo * Catalog::resolveModuleById(int id) {
    return selectSongInfo(QString("WHERE songs.id=%1").arg(id));
}

SongInfo * Catalog::resolveModuleByFileName(QString const& fileName) {
    return selectSongInfo(QString("WHERE songs.fileName='%1'").arg(fileName));
}

SongInfo * Catalog::readSongInfo(QSqlQuery &sqlQuery, QObject *parent) {
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
    return new SongInfo(id,
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

SongBasicInfo * Catalog::readSongBasicInfo(QSqlQuery &sqlQuery, QObject *parent) {
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

SongInfo * Catalog::selectSongInfo(QString const& whereClause) {
    QString query = QString(
                "SELECT"
                " songs.id AS id,"
                " fileName,"
                " songs.title as title,"
                " format AS formatId, "
                " downloads, "
                " favourited, "
                " score, "
                " size, "
                " length, "
                " playCount, "
                " lastPlayed, "
                " myFavourite, "
                " formats.description AS format, "
                " trackers.name AS tracker, "
                " genre, "
                " artist, "
                " patterns, "
                " orders, "
                " instruments, "
                " samples, "
                " channels "
                "FROM songs"
                " INNER JOIN trackers ON trackers.id=songs.tracker "
                " INNER JOIN formats ON formats.id=songs.format ");
    if(whereClause.length() > 0) {
        query += whereClause;
    }
    SongInfo * song = NULL;
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    if(sqlQuery.next()) {
        song = readSongInfo(sqlQuery, NULL); //TODO: leak
    }
    return song;
}

DataModel * Catalog::selectSongBasicInfo(QString const& whereClause,
                                         QString const& orderByClause) {
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
    typedef QListDataModel<SongBasicInfo*> SongMostDownloadedModel;
    SongMostDownloadedModel * model = new SongMostDownloadedModel();
    QSqlDatabase db = m_dataAccess->connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        model->append(readSongBasicInfo(sqlQuery, model));
    }
    return model;
}

DataModel * Catalog::findMostDownloadedSongs() {
    return selectSongBasicInfo("",
                               " ORDER BY downloads DESC ");
}

DataModel * Catalog::findMostFavouritedSongs() {
    return selectSongBasicInfo("",
                               " ORDER BY favourited DESC, downloads DESC, score DESC ");
}

DataModel * Catalog::findMostScoredSongs() {
    return selectSongBasicInfo("",
                               " ORDER BY score DESC, downloads DESC, favourited DESC ");
}

DataModel * Catalog::findRecentlyPlayedSongs() {
    return selectSongBasicInfo(" WHERE lastPlayed>0 ",
                               " ORDER BY lastPlayed DESC ");
}

DataModel * Catalog::findMyFavouriteSongs() {
    return selectSongBasicInfo(" WHERE myFavourite>0 ",
                               " ORDER BY playCount DESC, lastPlayed DESC ");
}

DataModel * Catalog::findMostPlayedSongs() {
    return selectSongBasicInfo(" WHERE playCount>0 ",
                               " ORDER BY playCount DESC, lastPlayed DESC ");
}

void Catalog::addFavourite(QVariant const& song) {
    qDebug() << "Adding favourite module" << song;

    QString query = QString("UPDATE songs SET myFavourite=myFavourite+1 ");
    if(song.type() == QVariant::Int)
    {
        query += QString("WHERE id=%1").arg(song.toInt());
    }
    else if(song.type() == QVariant::String)
    {
        query += QString("WHERE fileName='%1'").arg(song.toString());
    }
    else if((int)song.type() == (int)QMetaType::QObjectStar)
    {
        SongBasicInfo * info = qobject_cast<SongBasicInfo*>(song.value<QObject*>());
        query += QString("WHERE id=%1").arg(info->modId());
    }
    else
    {
        query = "";
    }

    if(query.length() > 0) {
        m_dataAccess->execute(query);
    }
}

void Catalog::removeFavourite(QVariant const& song) {
    qDebug() << "Removing favourite module" << song;

    QString query = QString("UPDATE songs SET myFavourite=0 ");
    if(song.type() == QVariant::Int)
    {
        query += QString("WHERE id=%1").arg(song.toInt());
    }
    else if(song.type() == QVariant::String)
    {
        query += QString("WHERE fileName='%1'").arg(song.toString());
    }
    else if((int)song.type() == (int)QMetaType::QObjectStar)
    {
        SongBasicInfo * info = qobject_cast<SongBasicInfo*>(song.value<QObject*>());
        query += QString("WHERE id=%1").arg(info->modId());
    }
    else
    {
        query = "";
    }

    if(query.length() > 0) {
        m_dataAccess->execute(query);
    }
}

void Catalog::play(QVariant const& song) {
    qDebug() << "Updating play count for module" << song;
    uint now = QDateTime::currentDateTime().toTime_t();
    QString query = QString("UPDATE songs SET playCount=playCount+1, lastPlayed=%1 ").arg(now);
    if(song.type() == QVariant::Int)
    {
        query += QString("WHERE id=%1").arg(song.toInt());
    }
    else if(song.type() == QVariant::String)
    {
        query += QString("WHERE fileName='%1'").arg(song.toString());
    }
    else if((int)song.type() == (int)QMetaType::QObjectStar)
    {
        SongBasicInfo * info = qobject_cast<SongBasicInfo*>(song.value<QObject*>());
        query += QString("WHERE id=%1").arg(info->modId());
    }
    else
    {
        query = "";
    }

    if(query.length() > 0) {
        m_dataAccess->execute(query);
    }
}

void Catalog::resetPlayCounts() {
    qDebug() << "Reset play counts";
    QString query = QString("UPDATE songs "
                            "SET playCount=0,"
                            "    lastPlayed=0 "
                            "WHERE playCount>0 OR lastPlayed>0");
    m_dataAccess->execute(query);
}

void Catalog::resetMyFavourites() {
    qDebug() << "Reset my favourites";
    QString query = QString("UPDATE songs "
                            "SET myFavourite=0 "
                            "WHERE myFavourite>0");
    m_dataAccess->execute(query);
}
