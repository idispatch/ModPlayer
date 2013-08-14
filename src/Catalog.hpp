#ifndef CATALOG_HPP_
#define CATALOG_HPP_

#include <QObject>
#include <QVariantList>

namespace bb {
    namespace cascades {
        class DataModel;
    }
    namespace data {
        class SqlDataAccess;
    }
}

class QSqlQuery;

class SongInfo;
class SongBasicInfo;
class Artist;

class Catalog : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString catalogPath READ catalogPath NOTIFY catalogPathChanged FINAL)
    Q_PROPERTY(bb::cascades::DataModel * formats READ formats NOTIFY formatsChanged FINAL)
    Q_PROPERTY(bb::cascades::DataModel * genres READ genres NOTIFY genresChanged FINAL)
    Q_PROPERTY(bb::cascades::DataModel * artists READ artists NOTIFY artistsChanged FINAL)
public:
    Catalog(QObject * parent = 0);

    QString catalogPath() const;

    bb::cascades::DataModel * formats();
    bb::cascades::DataModel * genres();
    bb::cascades::DataModel * artists();

    Q_INVOKABLE bb::cascades::DataModel * findSongsByFormatId(int formatId);
    Q_INVOKABLE bb::cascades::DataModel * findSongsByGenreId(int genreId);
    Q_INVOKABLE bb::cascades::DataModel * findSongsByArtistId(int artistId);

    Q_INVOKABLE bb::cascades::DataModel * findMostDownloadedSongs();
    Q_INVOKABLE bb::cascades::DataModel * findMostFavouritedSongs();
    Q_INVOKABLE bb::cascades::DataModel * findMostScoredSongs();

    Q_INVOKABLE bb::cascades::DataModel * findRecentlyPlayedSongs();
    Q_INVOKABLE bb::cascades::DataModel * findMyFavouriteSongs();
    Q_INVOKABLE bb::cascades::DataModel * findMostPlayedSongs();

    Q_INVOKABLE int resolveModuleIdByFileName(QString const& fileName);
    Q_INVOKABLE QString resolveFileNameById(int id);
    Q_INVOKABLE SongInfo * resolveModuleById(int id);
    Q_INVOKABLE SongInfo * resolveModuleByFileName(QString const& fileName);

    Q_INVOKABLE void addFavourite(QVariant const& song);
    Q_INVOKABLE void removeFavourite(QVariant const& song);
    Q_INVOKABLE void play(QVariant const& song);

    Q_INVOKABLE void resetPlayCounts();
    Q_INVOKABLE void resetMyFavourites();
Q_SIGNALS:
    void catalogPathChanged();
    void formatsChanged();
    void genresChanged();
    void artistsChanged();
private:
    Q_DISABLE_COPY(Catalog)
    void initCatalog();
    void copyCatalogToDataFolder();
    SongInfo * selectSongInfo(QString const& whereClause);
    bb::cascades::DataModel * selectSongBasicInfo(QString const& whereClause,
                                                  QString const& orderByClause);
    static SongBasicInfo * readSongBasicInfo(QSqlQuery &sqlQuery,
                                             QObject *parent);
    static SongInfo * readSongInfo(QSqlQuery &sqlQuery,
                                   QObject *parent);
private:
    bb::data::SqlDataAccess * m_dataAccess;
};

Q_DECLARE_METATYPE(Catalog*);

#endif
