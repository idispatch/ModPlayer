#ifndef CATALOG_HPP_
#define CATALOG_HPP_

#include <QObject>
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QVariant>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ArrayDataModel>
#include "SongFormat.hpp"
#include "SongGenre.hpp"
#include "SongBasicInfo.hpp"
#include "SongExtendedInfo.hpp"
#include "Artist.hpp"
#include "NamedPlaylist.hpp"
#include "Album.hpp"
#include "Radio.hpp"
#include "InstanceCounter.hpp"

#ifdef _DEBUG
//#define VERBOSE_LOGGING
#endif

namespace bb {
    namespace data {
        class SqlDataAccess;
    }
}

class QSqlQuery;
class SongBasicInfo;
class SongExtendedInfo;
class Artist;

class LocalSongInfo {
    int m_id;
    QString m_filePath;
public:
    LocalSongInfo() : m_id(0) {
    }
    int id() const {
        return m_id;
    }
    LocalSongInfo& setId(int value) {
        m_id = value;
        return *this;
    }
    QString const& filePath() const {
        return m_filePath;
    }
    LocalSongInfo& setFilePath(QString const& value) {
        m_filePath = value;
        return *this;
    }
};

class Catalog : public QThread,
                public InstanceCounter<Catalog> {
    Q_OBJECT
    Q_DISABLE_COPY(Catalog)
    Q_PROPERTY(QString catalogPath READ catalogPath NOTIFY catalogPathChanged FINAL)
    Q_PROPERTY(int songCount READ songCount NOTIFY songCountChanged FINAL)
public:
    static int Version;
    Catalog(QObject * parent);
    ~Catalog();

    void run();
    void stopThread();

    int songCount();
    Q_INVOKABLE int songCountAsync();

    int personalSongCount();
    Q_INVOKABLE int personalSongCountAsync();

    bool getLocalSongs(std::vector<LocalSongInfo> & songs);
    void deleteSong(int songId);

    QString catalogPath() const;

    Q_INVOKABLE bb::cascades::ArrayDataModel* findFormats();
    Q_INVOKABLE int findFormatsAsync();

    Q_INVOKABLE bb::cascades::GroupDataModel* findGenres(QString const& searchTerm);
    Q_INVOKABLE int findGenresAsync(QString const& searchTerm);

    Q_INVOKABLE bb::cascades::GroupDataModel* findArtists(QString const& searchTerm);
    Q_INVOKABLE int findArtistsAsync(QString const& searchTerm);

    Q_INVOKABLE bb::cascades::GroupDataModel* findPlaylists(QString const& searchTerm);
    Q_INVOKABLE int findPlaylistsAsync(QString const& searchTerm);

    Q_INVOKABLE bb::cascades::GroupDataModel* findAlbums(QString const& searchTerm);
    Q_INVOKABLE int findAlbumsAsync(QString const& searchTerm);

    Q_INVOKABLE bb::cascades::GroupDataModel* findDigitallyImported();
    Q_INVOKABLE bb::cascades::GroupDataModel* findSkyFm();
    Q_INVOKABLE bb::cascades::GroupDataModel* findJazzRadio();
    Q_INVOKABLE bb::cascades::GroupDataModel* findRockRadio();
    Q_INVOKABLE bb::cascades::GroupDataModel* findFrescaRadio();

    Q_INVOKABLE bb::cascades::ArrayDataModel* findSongsByFormatId(QString const& searchTerm, int formatId, int limit);
    Q_INVOKABLE int findSongsByFormatIdAsync(QString const& searchTerm, int formatId, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findSongsByGenreId(QString const& searchTerm, int genreId, int limit);
    Q_INVOKABLE int findSongsByGenreIdAsync(QString const& searchTerm, int genreId, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findSongsByArtistId(QString const& searchTerm, int artistId, int limit);
    Q_INVOKABLE int findSongsByArtistIdAsync(QString const& searchTerm, int artistId, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findSongsByPlaylistId(QString const& searchTerm, int playlistId, int limit);
    Q_INVOKABLE int findSongsByPlaylistIdAsync(QString const& searchTerm, int playlistId, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findSongsByAlbumId(QString const& searchTerm, int albumId, int limit);
    Q_INVOKABLE int findSongsByAlbumIdAsync(QString const& searchTerm, int albumId, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findSongsBySongAlbumId(QString const& searchTerm, int songId, int limit);
    Q_INVOKABLE int findSongsBySongAlbumIdAsync(QString const& searchTerm, int songId, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* searchSongs(QString const& searchTerm, int limit);
    Q_INVOKABLE int searchSongsAsync(QString const& searchTerm, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostDownloadedSongs(QString const& searchTerm, int limit);
    Q_INVOKABLE int findMostDownloadedSongsAsync(QString const& searchTerm, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostFavouritedSongs(QString const& searchTerm, int limit);
    Q_INVOKABLE int findMostFavouritedSongsAsync(QString const& searchTerm, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMyLocalSongs(QString const& searchTerm, int limit);
    Q_INVOKABLE int findMyLocalSongsAsync(QString const& searchTerm, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostScoredSongs(QString const& searchTerm, int limit);
    Q_INVOKABLE int findMostScoredSongsAsync(QString const& searchTerm, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findRecentlyPlayedSongs(QString const& searchTerm, int limit);
    Q_INVOKABLE int findRecentlyPlayedSongsAsync(QString const& searchTerm, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMyFavouriteSongs(QString const& searchTerm, int limit);
    Q_INVOKABLE int findMyFavouriteSongsAsync(QString const& searchTerm, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostPlayedSongs(QString const& searchTerm, int limit);
    Q_INVOKABLE int findMostPlayedSongsAsync(QString const& searchTerm, int limit);

    Q_INVOKABLE bb::cascades::GroupDataModel* findLiveStreamRadio(QString const& searchTerm,
                                                                  QString const& country,
                                                                  int limit);
    Q_INVOKABLE int findLiveStreamRadioAsync(QString const& searchTerm,
                                             QString const& country,
                                             int limit);

    // Synchronous only
    Q_INVOKABLE int resolveModuleIdByFileName(QString const& fileName);
    Q_INVOKABLE QString resolveFileNameById(int id);
    Q_INVOKABLE int resolveAlbumIdBySongId(int songId);
    Q_INVOKABLE QString resolveAlbumNameBySongId(int songId);
    Q_INVOKABLE SongExtendedInfo * resolveModuleById(int id, QVariant parent);
    Q_INVOKABLE SongExtendedInfo * resolveModuleByFileName(QString const& fileName, QVariant parent);

    Q_INVOKABLE void addFavourite(QVariant value);
    Q_INVOKABLE void removeFavourite(QVariant value);
    Q_INVOKABLE void play(QVariant value);

    Q_INVOKABLE void resetPlayCounts();
    Q_INVOKABLE void resetMyFavourites();

    Q_INVOKABLE void houseKeep();
    Q_INVOKABLE void addPersonalSong(SongExtendedInfo const& info);

    Q_INVOKABLE int createPlaylist(QString const& name);
    Q_INVOKABLE void deletePlaylistById(int playlistId);
    Q_INVOKABLE void deletePlaylistByName(QString const& name);
    Q_INVOKABLE void deleteAllPlaylists();
    Q_INVOKABLE void appendToPlaylist(int playlistId, int songId);
    Q_INVOKABLE void appendSongsToPlaylist(int playlistId, QVariantList songs);
    Q_INVOKABLE void deleteSongFromPlaylist(int playlistId, int songId);
    Q_INVOKABLE QVariant getPlaylistSongs(int playlistId);


    Q_INVOKABLE int createAlbum(QString const& artistName, QString const& albumName);
    Q_INVOKABLE QVariant getAlbumSongs(int albumId);

    Q_INVOKABLE int createFormat(QString const& name, QString const& description);
    Q_INVOKABLE int createGenre(QString const& name);
    Q_INVOKABLE int createArtist(QString const& name);

    Q_INVOKABLE void addSongToAlbum(int albumId, int songId, int trackNumber);

    using InstanceCounter<Catalog>::getInstanceCount;
    using InstanceCounter<Catalog>::getMaxInstanceCount;
Q_SIGNALS:
    void resultReady(int requestId, QVariant result);
    void songCountChanged();
    void catalogPathChanged();
private:
    void initCatalog();
    void copyCatalogToDataFolder();
    bb::cascades::GroupDataModel* findInternetRadio(QString const & channelList);

    SongExtendedInfo * selectSongInfo(QString const& whereClause, QObject *parent);

    bb::cascades::ArrayDataModel* selectSongBasicInfo(QString const& selectClause,
                                                      QString const& whereClause,
                                                      QString const& orderByClause,
                                                      int limit);

    bb::cascades::GroupDataModel* selectRadioInfo(QString const& selectClause,
                                                  QString const& whereClause,
                                                  QString const& orderByClause,
                                                  int limit);

    static SongBasicInfo * readSongBasicInfo(QSqlQuery &sqlQuery,
                                             QObject *parent);

    static SongExtendedInfo * readSongInfo(QSqlQuery &sqlQuery,
                                           QObject *parent);

    static Radio* readRadioInfo(QSqlQuery &sqlQuery, QObject *parent);
private:
    class Command {
    public:
        enum CommandType
        {
            SongCount,
            PersonalSongCount,
            SearchSongs,

            FormatsList,
            GenresList,
            ArtistsList,
            Playlists,
            Albums,
            Radio,

            SongsByFormatList,
            SongsByGenreList,
            SongsByArtistList,
            SongsByPlaylist,
            SongsByAlbum,
            SongsBySongAlbum,

            MostDownloadedSongs,
            MostFavouritedSongs,
            MostScoredSongs,

            RecentlyPlayedSongs,
            MyFavouriteSongs,
            MyLocalSongs,
            MostPlayedSongs,

            ExitCommand
        };
        Command(CommandType command)
            : m_command(command),
              m_id(nextCommandId()),
              m_thread(QThread::currentThread())
        {}
        virtual ~Command()
        {}
        inline int id() const {
            return m_id;
        }
        inline CommandType command() const {
            return m_command;
        }
        inline QThread* thread() const {
            return m_thread;
        }
    public:
        static int nextCommandId() {
            return ++s_commandCounter;
        }
    private:
        static int s_commandCounter;
        const CommandType m_command;
        const int m_id;
        QThread * m_thread;
    };

    class SearchCommand : public Command {
    public:
        SearchCommand(Command::CommandType commandType,
                      QString const& query,
                      int queryId,
                      int limit)
                    : Command(commandType),
                      m_query(query),
                      m_queryId(queryId),
                      m_limit(limit) {
                }
        QString const& query() const {
            return m_query;
        }
        int queryId() const {
            return m_queryId;
        }
        int limit() const {
            return m_limit;
        }
    private:
        const QString m_query;
        const int m_queryId;
        const int m_limit;
    };

    class SearchRadioCommand : public SearchCommand {
    public:
        SearchRadioCommand(Command::CommandType commandType,
                           QString const& query,
                           QString const& country,
                           int queryId,
                           int limit)
                        : SearchCommand(commandType, query, queryId, limit),
                          m_country(country) {
                }
        QString const& country() const {
            return m_country;
        }
    private:
        const QString m_country;
    };
private:
    int asyncCommandSubmit(Command * command);
    int asyncCommand(Command::CommandType commandType);
    int asyncSearchCommand(Command::CommandType commandType,
                           QString const& searchTerm,
                           int queryId,
                           int limit);
    int asyncSearchRadioCommand(Command::CommandType commandType,
                                QString const& searchTerm,
                                QString const& country,
                                int queryId,
                                int limit);

    QVariant assign(std::auto_ptr<Command> &command,
                    bb::cascades::DataModel * value) {
        value->moveToThread(command->thread());
        return QVariant::fromValue(value);
    }
    static QString escapeSql(QString const& value);
private:
    QQueue<Command*> m_commandQueue;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bb::data::SqlDataAccess * m_dataAccess;
};

Q_DECLARE_METATYPE(Catalog*);

#endif
