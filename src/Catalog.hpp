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

class Catalog : public QThread,
                public InstanceCounter<Catalog> {
    Q_OBJECT
    Q_PROPERTY(QString catalogPath READ catalogPath NOTIFY catalogPathChanged FINAL)
    Q_PROPERTY(int songCount READ songCount NOTIFY songCountChanged FINAL)
public:
    Catalog(QObject * parent);

    void run();
    void stopThread();

    int songCount();
    Q_INVOKABLE int songCountAsync();

    int personalSongCount();
    Q_INVOKABLE int personalSongCountAsync();

    QString catalogPath() const;

    Q_INVOKABLE bb::cascades::ArrayDataModel* findFormats();
    Q_INVOKABLE int findFormatsAsync();

    Q_INVOKABLE bb::cascades::GroupDataModel* findGenres();
    Q_INVOKABLE int findGenresAsync();

    Q_INVOKABLE bb::cascades::GroupDataModel* findArtists();
    Q_INVOKABLE int findArtistsAsync();

    Q_INVOKABLE bb::cascades::GroupDataModel* findPlaylists();
    Q_INVOKABLE int findPlaylistsAsync();

    Q_INVOKABLE bb::cascades::ArrayDataModel* findSongsByFormatId(int formatId, int limit);
    Q_INVOKABLE int findSongsByFormatIdAsync(int formatId, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findSongsByGenreId(int genreId, int limit);
    Q_INVOKABLE int findSongsByGenreIdAsync(int genreId, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findSongsByArtistId(int artistId, int limit);
    Q_INVOKABLE int findSongsByArtistIdAsync(int artistId, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* searchSongs(QString const& searchTerm, int limit);
    Q_INVOKABLE int searchSongsAsync(QString const& searchTerm, int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostDownloadedSongs(int limit);
    Q_INVOKABLE int findMostDownloadedSongsAsync(int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostFavouritedSongs(int limit);
    Q_INVOKABLE int findMostFavouritedSongsAsync(int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMyLocalSongs(int limit);
    Q_INVOKABLE int findMyLocalSongsAsync(int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostScoredSongs(int limit);
    Q_INVOKABLE int findMostScoredSongsAsync(int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findRecentlyPlayedSongs(int limit);
    Q_INVOKABLE int findRecentlyPlayedSongsAsync(int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMyFavouriteSongs(int limit);
    Q_INVOKABLE int findMyFavouriteSongsAsync(int limit);

    Q_INVOKABLE bb::cascades::ArrayDataModel* findMostPlayedSongs(int limit);
    Q_INVOKABLE int findMostPlayedSongsAsync(int limit);

    // Synchronous only
    Q_INVOKABLE int resolveModuleIdByFileName(QString const& fileName);
    Q_INVOKABLE QString resolveFileNameById(int id);
    Q_INVOKABLE SongExtendedInfo * resolveModuleById(int id, QVariant parent);
    Q_INVOKABLE SongExtendedInfo * resolveModuleByFileName(QString const& fileName, QVariant parent);

    Q_INVOKABLE void addFavourite(QVariant value);
    Q_INVOKABLE void removeFavourite(QVariant value);
    Q_INVOKABLE void play(QVariant value);

    Q_INVOKABLE void resetPlayCounts();
    Q_INVOKABLE void resetMyFavourites();

    Q_INVOKABLE void clearPersonalSongs();
    Q_INVOKABLE void addPersonalSong(SongExtendedInfo const& info);

    Q_INVOKABLE int createPlaylist(QString const& name);
    Q_INVOKABLE void deletePlaylist(int playlistId);

    using InstanceCounter<Catalog>::getInstanceCount;
    using InstanceCounter<Catalog>::getMaxInstanceCount;
Q_SIGNALS:
    void resultReady(int requestId, QVariant result);
    void songCountChanged();
    void catalogPathChanged();
private:
    Q_DISABLE_COPY(Catalog)
    void initCatalog();
    void copyCatalogToDataFolder();

    SongExtendedInfo * selectSongInfo(QString const& whereClause, QObject *parent);

    bb::cascades::ArrayDataModel* selectSongBasicInfo(QString const& whereClause,
                                                      QString const& orderByClause,
                                                      int limit);

    static SongBasicInfo * readSongBasicInfo(QSqlQuery &sqlQuery,
                                             QObject *parent);

    static SongExtendedInfo * readSongInfo(QSqlQuery &sqlQuery,
                                           QObject *parent);
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

            SongsByFormatList,
            SongsByGenreList,
            SongsByArtistList,

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

    class FindCommand : public Command {
    public:
        FindCommand(CommandType command, int queryId, int limit)
            : Command(command),
              m_queryId(queryId),
              m_limit(limit) {
        }
        int queryId() const {
            return m_queryId;
        }
        int limit() const {
            return m_limit;
        }
    private:
        const int m_queryId;
        const int m_limit;
    };

    class SearchCommand : public Command {
    public:
        SearchCommand(QString const& query, int limit)
            : Command(Command::SearchSongs),
              m_query(query),
              m_limit(limit) {
        }
        QString const& query() const {
            return m_query;
        }
        int limit() const {
            return m_limit;
        }
    private:
        const QString m_query;
        const int m_limit;
    };
private:
    int asyncCommandSubmit(Command * command);
    int asyncCommand(Command::CommandType commandType);
    int asyncFindCommand(Command::CommandType commandType, int id, int limit);
    int asyncSearchCommand(QString const& query, int limit);
private:
    QQueue<Command*> m_commandQueue;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bb::data::SqlDataAccess * m_dataAccess;
};

Q_DECLARE_METATYPE(Catalog*);

#endif
