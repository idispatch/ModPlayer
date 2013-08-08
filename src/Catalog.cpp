#include "Catalog.hpp"
#include "SongFormat.hpp"
#include "SongGenre.hpp"
#include "SongBasicInfo.hpp"

#include  <QDebug>

#include  <QtSql/QSqlDatabase>
#include  <QtSql/QSqlQuery>

#include  <bb/data/SqlDataAccess>
#include  <bb/data/DataSource>
#include  <bb/cascades/GroupDataModel>
#include  <bb/cascades/QListDataModel>

using namespace bb::data;
using namespace bb::cascades;

//#define DEBUG_CATALOG

Catalog::Catalog(QObject * parent)
    : QObject(parent),
      m_dataAccess(NULL) {
    init();
}

void Catalog::init() {
    m_dataAccess = new SqlDataAccess("app/native/assets/catalog.sqlite", "catalog", this);

    DataSource::registerQmlTypes();
    qmlRegisterUncreatableType<SongFormat>("player", 1, 0, "SongFormat", "");
    qmlRegisterUncreatableType<SongGenre>("player", 1, 0, "SongGenre", "");
    qmlRegisterUncreatableType<SongBasicInfo>("player", 1, 0, "SongBasicInfo", "");
}

void Catalog::dumpData(QVariantList const& data) {
#if 0
#ifdef DEBUG_CATALOG
    int c = data.count();
    for(int i = 0; i< c;++i){
        QVariant v = data[i];
        QMap<QString, QVariant> m = v.toMap();
        QList<QString> keys = m.keys();
        for(int j = keys.count() - 1; j>=0; --j){
            qDebug() << keys[j] << ":" << m[keys[j]];
        }
    }
#endif
#else
    Q_UNUSED(data);
#endif
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
            " COUNT(songs.id) "
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

DataModel * Catalog::findSongsByFormatId(int formatId) {
    QString query = QString("SELECT"
                            " id, "
                            " fileName, "
                            " title, "
                            " downloads, "
                            " favourited, "
                            " score, "
                            " size, "
                            " length "
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
            " downloads, "
            " favourited, "
            " score, "
            " size, "
            " length "
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
    dumpData(data);
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
    dumpData(data);
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

QVariant Catalog::resolveModuleById(int id) {
#ifdef DEBUG_CATALOG
    qDebug() << "Resolving module id" << id;
#endif
    QString query = QString(
            "SELECT"
            " songs.id AS id,"
            " fileName,"
            " size, "
            " songs.title AS title, "
            " formats.name AS format, "
            " length, "
            " trackers.name AS tracker, "
            " genre, "
            " artist, "
            " downloads, "
            " favourited, "
            " score, "
            " md5, "
            " patterns, "
            " orders, "
            " instruments, "
            " samples, "
            " channels "
            "FROM songs "
            "INNER JOIN trackers ON trackers.id=songs.tracker "
            "INNER JOIN formats ON formats.id=songs.format "
            "WHERE songs.id=%1").arg(id);
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    dumpData(data);
    if(data.size() == 1)
    {
        QVariant const& first = data.first();
        return first;
    }
    else
    {
#ifdef DEBUG_CATALOG
        qDebug() << "Module" << id << "cannot be resolved";
#endif
        return QVariant();
    }
}

QVariant Catalog::resolveModuleByFileName(QString const& fileName) {
#ifdef DEBUG_CATALOG
    qDebug() << "Resolving module file name" << fileName;
#endif
    QString query = QString(
            "SELECT"
            " songs.id AS id,"
            " fileName,"
            " size, "
            " songs.title AS title, "
            " formats.name AS format, "
            " length, "
            " trackers.name AS tracker, "
            " genre, "
            " artist, "
            " downloads, "
            " favourited, "
            " score, "
            " md5, "
            " patterns, "
            " orders, "
            " instruments, "
            " samples, "
            " channels "
            "FROM songs "
            "INNER JOIN trackers ON trackers.id=songs.tracker "
            "INNER JOIN formats ON formats.id=songs.format "
            "WHERE songs.fileName='%1'").arg(fileName);
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    dumpData(data);
    if(data.size() == 1)
    {
        QVariant const& first = data.first();
        return first;
    }
    else
    {
#ifdef DEBUG_CATALOG
        qDebug() << "Module file name" << fileName << "cannot be resolved";
#endif
        return QVariant();
    }
}

SongBasicInfo * Catalog::readSongBasicInfo(QSqlQuery &sqlQuery, QObject *parent) {
    int column       = 0;
    int id           = sqlQuery.value(column++).toInt();
    QString fileName = sqlQuery.value(column++).toString();
    QString title    = sqlQuery.value(column++).toString();
    int downloads    = sqlQuery.value(column++).toInt();
    int favourited   = sqlQuery.value(column++).toInt();
    int score        = sqlQuery.value(column++).toInt();
    int size         = sqlQuery.value(column++).toInt();
    int length       = sqlQuery.value(column++).toInt();
    return new SongBasicInfo(id,
                             fileName,
                             title,
                             downloads,
                             favourited,
                             score,
                             size,
                             length,
                             parent);
}

DataModel * Catalog::selectSongBasicInfoObjects(const char * query) {
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
    const char * query = "SELECT"
                         " id, "
                         " fileName, "
                         " title, "
                         " downloads, "
                         " favourited, "
                         " score, "
                         " size, "
                         " length "
                         "FROM songs "
                         "ORDER BY downloads DESC";
    return selectSongBasicInfoObjects(query);
}

DataModel * Catalog::findMostFavouritedSongs() {
    const char * query = "SELECT"
                         " id, "
                         " fileName, "
                         " title, "
                         " downloads, "
                         " favourited, "
                         " score, "
                         " size, "
                         " length "
                         "FROM songs "
                         "ORDER BY favourited DESC, downloads DESC, score DESC";
    return selectSongBasicInfoObjects(query);
}

DataModel * Catalog::findMostScoredSongs() {
    const char * query = "SELECT"
                         " id, "
                         " fileName, "
                         " title, "
                         " downloads, "
                         " favourited, "
                         " score, "
                         " size, "
                         " length "
                         "FROM songs "
                         "ORDER BY score DESC, downloads DESC, favourited DESC";
    return selectSongBasicInfoObjects(query);
}

void Catalog::addFavourite(QVariant const& song) {
    qDebug() << "Adding favourite module id" << song;
}

void Catalog::removeFavourite(QVariant const& song) {
    qDebug() << "Removing favourite module id" << song;
}

void Catalog::play(QVariant const& song) {
    qDebug() << "Updating play count for module id" << song;
}
