#ifndef CATALOGMIGRATION_HPP_
#define CATALOGMIGRATION_HPP_

#include <QString>
#include <QVector>
#include <bb/data/SqlDataAccess>

struct CatalogMigrationData {
    struct V1 {
        struct SongsTable {
            int id;
            int playCount;
            int lastPlayed;
            int myFavourite;
        };
        typedef QVector<SongsTable> SongTableData;
    };
    struct V2 {
        struct SongsTable {
            int id;
            QString fileName;
            int size;
            QString title;

            int format;
            int length;
            int tracker;
            int genre;
            int artist;

            int downloads;
            int favourited;
            int score;

            int playCount;
            int lastPlayed;
            int myFavourite;

            int patterns;
            int orders;
            int instruments;
            int samples;
            int channels;
        };
        struct ArtistsTable {
            int id;
            QString name;
        };
        struct GenresTable {
            int id;
            QString name;
        };
        struct PlaylistsTable {
            int id;
            QString name;
        };
        struct PlaylistEntriesTable {
            int playlistId;
            int songId;
            int songOrder;
        };
        typedef QVector<SongsTable>             SongTableData;
        typedef QVector<ArtistsTable>           ArtistsTableData;
        typedef QVector<GenresTable>            GenresTableData;
        typedef QVector<PlaylistsTable>         PlaylistTableData;
        typedef QVector<PlaylistEntriesTable>   PlaylistEntriesTableData;
    };
    struct V3 {
        struct AlbumsTable {
            int id;
            int artistId;
            QString name;
        };

        struct AlbumEntriesTable {
            int albumId;
            int songId;
            int trackNumber;
        };

        typedef QVector<AlbumsTable>            AlbumsTableData;
        typedef QVector<AlbumEntriesTable>      AlbumEntriesTableData;
    };

    struct CatalogV1 {
        typedef V1::SongTableData SongTableData;

        SongTableData songs;

        size_t totalUpdates() const {
            return songs.size();
        }

        bool empty() const {
            return songs.empty();
        }
    };

    struct CatalogV2 {
        typedef V2::SongTableData               SongTableData;
        typedef V2::ArtistsTableData            ArtistsTableData;
        typedef V2::GenresTableData             GenresTableData;
        typedef V2::PlaylistTableData           PlaylistTableData;
        typedef V2::PlaylistEntriesTableData    PlaylistEntriesTableData;

        SongTableData               songs;
        ArtistsTableData            artists;
        GenresTableData             genres;
        PlaylistTableData           playlists;
        PlaylistEntriesTableData    playlistEntries;

        size_t totalUpdates() const {
            return songs.size() +
                   artists.size() +
                   genres.size() +
                   playlists.size() +
                   playlistEntries.size();
        }

        bool empty() const {
            return songs.empty() +
                   artists.empty() +
                   genres.empty() +
                   playlists.empty() +
                   playlistEntries.empty();
        }
    };

    struct CatalogV3 {
        typedef V3::AlbumsTableData             AlbumsTableData;
        typedef V3::AlbumEntriesTableData       AlbumEntriesTableData;

        AlbumsTableData             albums;
        AlbumEntriesTableData       albumEntries;

        size_t totalUpdates() const {
            return albums.size() +
                   albumEntries.size();
        }

        bool empty() const {
            return albums.empty() &&
                   albumEntries.empty();
        }
    };

    struct Catalog {
        int version;
        CatalogV1 v1;
        CatalogV2 v2;
        CatalogV3 v3;

        Catalog(int v = 0) : version(v) {}

        size_t totalUpdates() const {
            return v1.totalUpdates() +
                   v2.totalUpdates() +
                   v3.totalUpdates();
        }

        bool empty() const {
            return v1.empty() &&
                   v2.empty() &&
                   v3.empty();
        }
    };
};

inline
SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V1::SongsTable& song) {
    reader >> song.id
           >> song.playCount
           >> song.lastPlayed
           >> song.myFavourite;
    return reader;
}

inline
SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V2::SongsTable& song) {
    reader >> song.id
           >> song.fileName
           >> song.size
           >> song.title
           >> song.format
           >> song.length
           >> song.tracker
           >> song.genre
           >> song.artist
           >> song.downloads
           >> song.favourited
           >> song.score
           >> song.playCount
           >> song.lastPlayed
           >> song.myFavourite
           >> song.patterns
           >> song.orders
           >> song.instruments
           >> song.samples
           >> song.channels;
    return reader;
}

inline
SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V2::ArtistsTable& artist) {
    reader >> artist.id
           >> artist.name;
    return reader;
}

inline
SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V2::GenresTable& genre) {
    reader >> genre.id
           >> genre.name;
    return reader;
}

inline
SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V2::PlaylistsTable& playlist) {
    reader >> playlist.id
           >> playlist.name;
    return reader;
}

inline
SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V2::PlaylistEntriesTable& playlistEntry) {
    reader >> playlistEntry.playlistId
           >> playlistEntry.songId
           >> playlistEntry.songOrder;
    return reader;
}

inline
SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V3::AlbumsTable& album) {
    reader >> album.id
           >> album.artistId
           >> album.name;
    return reader;
}

inline
SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V3::AlbumEntriesTable& album) {
    reader >> album.albumId
           >> album.songId
           >> album.trackNumber;
    return reader;
}

template<typename T>
inline
void readUserData(bb::data::SqlDataAccess& dataAccess, const char * query, T& data) {
    QSqlDatabase db = dataAccess.connection();
    QSqlQuery sqlQuery = db.exec(query);
    while(sqlQuery.next()) {
        SqlReader reader(sqlQuery);
        typename T::value_type value;
        reader >> value;
        data.push_back(value);
    }
}

inline
bool tableExists(bb::data::SqlDataAccess& dataAccess, const char * tableName) {
    int count = 0;
    QSqlDatabase db = dataAccess.connection();
    QString query = QString("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='%1'").arg(tableName);
    QSqlQuery sqlQuery = db.exec(query);
    if(sqlQuery.next()) {
        SqlReader reader(sqlQuery);
        reader >> count;
    }
    return count > 0;
}

#endif
