#include "Catalog.hpp"

#include  <QDebug>
#include  <bb/data/SqlDataAccess>
#include  <bb/data/DataSource>
#include  <bb/cascades/GroupDataModel>
#include  <bb/cascades/QListDataModel>

using namespace bb::data;
using namespace bb::cascades;

Catalog::Catalog(QObject * parent)
    : QObject(parent),
      m_dataAccess(new SqlDataAccess("app/native/assets/catalog.sqlite", "catalog", this)) {
    DataSource::registerQmlTypes();
}

void Catalog::dumpData(QVariantList const& data) {
#if 0
    int c = data.count();
    for(int i = 0; i< c;++i){
        QVariant v = data[i];
        QMap<QString, QVariant> m = v.toMap();
        QList<QString> keys = m.keys();
        for(int j = keys.count() - 1; j>=0; --j){
            qDebug() << keys[j] << ":" << m[keys[j]];
        }
    }
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
            "WHERE format=%1 "
            "ORDER BY downloads").arg(formatId);
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    //dumpData(data);
    QStringList keys;
    keys << "fileName" << "downloads";
    GroupDataModel * model = new GroupDataModel(keys);
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);
    model->insertList(data);
    return model;
}

DataModel * Catalog::findSongsByGenreId(int genreId) {
    QString query = QString("SELECT id, fileName FROM songs WHERE genre=%1 ORDER BY num_downloads").arg(genreId);
    QVariantList data = m_dataAccess->execute(query).value<QVariantList>();
    //dumpData(data);
    QStringList keys;
    keys << "name";
    GroupDataModel * model = new GroupDataModel(keys);
    model->setGrouping(ItemGrouping::ByFirstChar);
    model->setSortedAscending(true);
    model->insertList(data);
    return model;
}
