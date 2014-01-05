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
        typedef QVector<PlaylistsTable>         PlaylistTableData;
        typedef QVector<PlaylistEntriesTable>   PlaylistEntriesTableData;
    };

    struct CatalogV1 {
        typedef V1::SongTableData SongTableData;

        SongTableData songs;
    };

    struct CatalogV2 {
        typedef V2::SongTableData               SongTableData;
        typedef V2::PlaylistTableData           PlaylistTableData;
        typedef V2::PlaylistEntriesTableData    PlaylistEntriesTableData;

        SongTableData               songs;
        PlaylistTableData           playlists;
        PlaylistEntriesTableData    playlistEntries;
    };

    struct Catalog {
        int version;
        CatalogV1 v1;
        CatalogV2 v2;

        Catalog(int v = 0) : version(v) {}

        bool empty() const {
            switch(version) {
            case 0:
            case 1:
                return v1.songs.empty();
            case 2:
                return v1.songs.empty() &&
                       v2.songs.empty() &&
                       v2.playlists.empty() &&
                       v2.playlistEntries.empty();
            default:
                return true;
            }
        }
    };
};

SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V1::SongsTable& song) {
    reader >> song.id
           >> song.playCount
           >> song.lastPlayed
           >> song.myFavourite;
    return reader;
}

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

SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V2::PlaylistsTable& playlist) {
    reader >> playlist.id
           >> playlist.name;
    return reader;
}

SqlReader& operator >> (SqlReader& reader, CatalogMigrationData::V2::PlaylistEntriesTable& playlistEntry) {
    reader >> playlistEntry.playlistId
           >> playlistEntry.songId
           >> playlistEntry.songOrder;
   return reader;
}

template<typename T>
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

#endif
