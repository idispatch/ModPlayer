#include "Catalog.hpp"

#include  <QDebug>
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
    DataSource::registerQmlTypes();
    init();
}

void Catalog::init() {
    m_dataAccess = new SqlDataAccess("app/native/assets/catalog.sqlite", "catalog", this);
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
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    QVariantListDataModel * model = new QVariantListDataModel(data);
    return model;
}

DataModel * Catalog::genres() {
    const char * query = "SELECT id, name FROM genres";
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    QStringList keys;
    keys << "name";
    GroupDataModel * model = new GroupDataModel(keys);
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);
    model->insertList(data);
    return model;
}

DataModel * Catalog::findSongsByFormatId(int formatId) {
    QString query = QString("SELECT"
            " id AS id, "
            " fileName AS fileName, "
            " title AS title, "
            " downloads AS downloads, "
            " favourited AS favourited, "
            " score AS score, "
            " size AS size, "
            " length AS length "
            "FROM songs "
            "WHERE format=%1").arg(formatId);
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    dumpData(data);
    QStringList keys;
    keys << "fileName" << "downloads";
    GroupDataModel * model = new GroupDataModel(keys);
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);
    model->insertList(data);
    return model;
}

DataModel * Catalog::findSongsByGenreId(int genreId) {
    QString query = QString(
            "SELECT "
            " id,"
            " fileName "
            "FROM songs "
            "WHERE genre=%1").arg(genreId);
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    dumpData(data);
    QStringList keys;
    keys << "name";
    GroupDataModel * model = new GroupDataModel(keys);
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);
    model->insertList(data);
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

DataModel * Catalog::findMostDownloadedSongs() {
    QString query = QString("SELECT"
                " id AS id, "
                " fileName AS fileName, "
                " title AS title, "
                " downloads AS downloads, "
                " favourited AS favourited, "
                " score AS score, "
                " size AS size, "
                " length AS length "
                "FROM songs "
                "ORDER BY downloads DESCENDING");
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    dumpData(data);
    QVariantListDataModel * model = new QVariantListDataModel(data);
    return model;
}

DataModel * Catalog::findMostFavouritedSongs() {
    QString query = QString("SELECT"
                    " id AS id, "
                    " fileName AS fileName, "
                    " title AS title, "
                    " downloads AS downloads, "
                    " favourited AS favourited, "
                    " score AS score, "
                    " size AS size, "
                    " length AS length "
                    "FROM songs "
                    "ORDER BY favourited DESCENDING");
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    dumpData(data);
    QVariantListDataModel * model = new QVariantListDataModel(data);
    return model;
}

DataModel * Catalog::findMostScoredSongs() {
    QString query = QString("SELECT"
                        " id AS id, "
                        " fileName AS fileName, "
                        " title AS title, "
                        " downloads AS downloads, "
                        " favourited AS favourited, "
                        " score AS score, "
                        " size AS size, "
                        " length AS length "
                        "FROM songs "
                        "ORDER BY score DESCENDING");
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    dumpData(data);
    QVariantListDataModel * model = new QVariantListDataModel(data);
    return model;
}
