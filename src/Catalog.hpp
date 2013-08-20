#ifndef CATALOG_HPP_
#define CATALOG_HPP_

#include <QObject>
#include <QVariant>
#include  <bb/cascades/GroupDataModel>
#include  <bb/cascades/ArrayDataModel>
#include "SongFormat.hpp"
#include "SongGenre.hpp"
#include "SongBasicInfo.hpp"
#include "SongExtendedInfo.hpp"
#include "Artist.hpp"
#include "InstanceCounter.hpp"

namespace bb {
    namespace data {
        class SqlDataAccess;
    }
}

class QSqlQuery;
class SongBasicInfo;
class SongExtendedInfo;
class Artist;

class Catalog : public QObject,
                public InstanceCounter<Catalog> {
    Q_OBJECT
    Q_PROPERTY(QString catalogPath READ catalogPath NOTIFY catalogPathChanged FINAL)
public:
    Catalog(QObject * parent);

    QString catalogPath() const;

    bb::cascades::ArrayDataModel* formats();
    bb::cascades::GroupDataModel* genres();
    bb::cascades::GroupDataModel* artists();

    Q_INVOKABLE bb::cascades::ArrayDataModel* findFormats(QVariant parent);
    Q_INVOKABLE bb::cascades::GroupDataModel* findGenres(QVariant parent);
    Q_INVOKABLE bb::cascades::GroupDataModel* findArtists(QVariant parent);

    Q_INVOKABLE bb::cascades::GroupDataModel* findSongsByFormatId(int formatId, QVariant parent);
    Q_INVOKABLE bb::cascades::GroupDataModel* findSongsByGenreId(int genreId, QVariant parent);
    Q_INVOKABLE bb::cascades::GroupDataModel* findSongsByArtistId(int artistId, QVariant parent);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostDownloadedSongs(QVariant parent);
    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostFavouritedSongs(QVariant parent);
    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostScoredSongs(QVariant parent);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findRecentlyPlayedSongs(QVariant parent);
    Q_INVOKABLE bb::cascades::ArrayDataModel* findMyFavouriteSongs(QVariant parent);
    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostPlayedSongs(QVariant parent);

    Q_INVOKABLE int resolveModuleIdByFileName(QString const& fileName);
    Q_INVOKABLE QString resolveFileNameById(int id);
    Q_INVOKABLE SongExtendedInfo * resolveModuleById(int id, QVariant parent);
    Q_INVOKABLE SongExtendedInfo * resolveModuleByFileName(QString const& fileName, QVariant parent);

    Q_INVOKABLE void addFavourite(QVariant value);
    Q_INVOKABLE void removeFavourite(QVariant value);
    Q_INVOKABLE void play(QVariant value);

    Q_INVOKABLE void resetPlayCounts();
    Q_INVOKABLE void resetMyFavourites();

    using InstanceCounter<Catalog>::getInstanceCount;
    using InstanceCounter<Catalog>::getMaxInstanceCount;
Q_SIGNALS:
    void catalogPathChanged();
private:
    Q_DISABLE_COPY(Catalog)
    void initCatalog();
    void copyCatalogToDataFolder();

    SongExtendedInfo * selectSongInfo(QString const& whereClause, QObject *parent);

    bb::cascades::ArrayDataModel* selectSongBasicInfo(QString const& whereClause,
                                                      QString const& orderByClause,
                                                      QObject *parent);

    static SongBasicInfo * readSongBasicInfo(QSqlQuery &sqlQuery,
                                             QObject *parent);

    static SongExtendedInfo * readSongInfo(QSqlQuery &sqlQuery,
                                           QObject *parent);
private:
    bb::data::SqlDataAccess * m_dataAccess;
};

Q_DECLARE_METATYPE(Catalog*);

#endif
