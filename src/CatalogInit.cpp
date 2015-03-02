#include "FileUtils.hpp"
#include "Catalog.hpp"
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <bb/data/SqlDataAccess>
#include <bb/data/DataSource>

#include "MessageBox.hpp"
#include "SqlReader.hpp"
#include "FileUtils.hpp"
#include "CatalogMigration.hpp"

using namespace bb::data;
using namespace bb::cascades;

namespace {
    const char * CATALOG_NAME = "catalog.sqlite";
}

void Catalog::initCatalog() {
    copyCatalogToDataFolder();
    m_dataAccess = new SqlDataAccess(catalogPath(), "catalog", this);
    emit upgradeCompleted();
}

QString Catalog::catalogPath() const {
    return FileUtils::joinPath(QDir::homePath(), CATALOG_NAME);
}

void Catalog::copyCatalogToDataFolder() {
    // If the catalog does not exist in app sandbox then the app was started the first time
    // Copy the initial catalog to the app sandbox
    if(!FileUtils::exists(catalogPath())) {

        qDebug() << "Personal catalog does not exist at" << catalogPath();

        MessageBox message(tr("Initializing Catalog"),
                           tr("Your song catalog is being initialized. Please wait."));

        QString appFolder(QDir::homePath());
        appFolder.chop(4);
        QString originalFileName = FileUtils::joinPath(appFolder, "app/native/assets");
        originalFileName = FileUtils::joinPath(originalFileName, CATALOG_NAME);

        if(FileUtils::exists(originalFileName)) {
            QFile originalFile(originalFileName);
            qDebug() << "Original catalog is at" << originalFileName;
            if(!originalFile.copy(catalogPath())) {
                qDebug() << "Failed to copy catalog to path: " << catalogPath();
            }
        } else {
            qDebug() << "Failed to copy catalog - file does not exists";
        }
    } else {
        // Catalog exists. Check the catalog version and perform upgrade if required.
        typedef CatalogMigrationData::Catalog MigrationData;
        MigrationData data;
        {
            {
                SqlDataAccess dataAccess(catalogPath(), "catalog");
                QSqlDatabase db = dataAccess.connection();
                QSqlQuery sqlQuery = db.exec("PRAGMA user_version");
                if(sqlQuery.next()) {
                    SqlReader(sqlQuery) >> data.version;
                }
            }

            if(data.version == Catalog::Version) {
                qDebug() << "Database migration not required, version" << data.version;
                QSqlDatabase::removeDatabase("catalog");
                return;
            } else {
                SqlDataAccess dataAccess(catalogPath(), "catalog");
                qDebug() << "Starting database migration from version"
                         << data.version
                         << "to version"
                         << Catalog::Version;

                MessageBox message(tr("Updating Catalog"),
                                   tr("Your song catalog is being updated. Please wait."));

                // All formats support play count, last played, my favorite
                if(data.version >= 0) {
                    const char * query = "SELECT id, playCount, lastPlayed, myFavourite "
                                         "FROM songs "
                                         "WHERE id > 0 AND (playCount > 0 OR lastPlayed > 0 OR myFavourite > 0)";
                    readUserData(dataAccess, query, data.v1.songs);
                    qDebug() << "Migration V1 read: songs=" << data.v1.songs.size();
                }

                // Format 2 and newer support user songs (and playlists, genres, artists)
                if(data.version >= 2) {
                    const char * query = "SELECT"
                                         " songs.id,"
                                         " songs.fileName,"
                                         " songs.size,"
                                         " songs.title,"
                                         " songs.format,"
                                         " songs.length,"
                                         " songs.tracker,"
                                         " songs.genre,"
                                         " songs.artist,"
                                         " songs.downloads,"
                                         " songs.favourited,"
                                         " songs.score,"
                                         " songs.playCount,"
                                         " songs.lastPlayed,"
                                         " songs.myFavourite,"
                                         " songs.patterns,"
                                         " songs.orders,"
                                         " songs.instruments,"
                                         " songs.samples,"
                                         " songs.channels "
                                         "FROM songs "
                                         " INNER JOIN trackers ON trackers.id=songs.tracker "
                                         " INNER JOIN formats ON formats.id=songs.format "
                                         " INNER JOIN artists ON artists.id=songs.artist "
                                         " INNER JOIN genres ON genres.id=songs.genre "
                                         "WHERE songs.id < 0 ";
                    readUserData(dataAccess, query, data.v2.songs);
                    qDebug() << "Migration V2 read: songs=" << data.v2.songs.size();

                    readUserData(dataAccess,
                                 "SELECT "
                                 " id,"
                                 " name "
                                 "FROM artists "
                                 "WHERE id < 0",
                                 data.v2.artists);
                    qDebug() << "Migration V2 read: artists=" << data.v2.artists.size();

                    readUserData(dataAccess,
                                 "SELECT "
                                 " id,"
                                 " name "
                                 "FROM genres "
                                 "WHERE id < 0",
                                 data.v2.genres);
                    qDebug() << "Migration V2 read: genres=" << data.v2.genres.size();

                    if(tableExists(dataAccess, "playlists")) {
                        readUserData(dataAccess,
                                     "SELECT "
                                     " id,"
                                     " name "
                                     "FROM playlists",
                                     data.v2.playlists);
                    }
                    qDebug() << "Migration V2 read: playlists=" << data.v2.playlists.size();

                    if(tableExists(dataAccess, "playlistEntries")) {
                        readUserData(dataAccess,
                                     "SELECT "
                                     " playlistEntries.playlistId,"
                                     " playlistEntries.songId,"
                                     " playlistEntries.songOrder "
                                     "FROM playlistEntries"
                                     " INNER JOIN playlists ON playlists.id=playlistEntries.playlistId "
                                     " INNER JOIN songs ON songs.id=playlistEntries.songId "
                                     " INNER JOIN trackers ON trackers.id=songs.tracker "
                                     " INNER JOIN formats ON formats.id=songs.format "
                                     " INNER JOIN artists ON artists.id=songs.artist "
                                     " INNER JOIN genres ON genres.id=songs.genre ",
                                     data.v2.playlistEntries);
                    }
                    qDebug() << "Migration V2 read: playlistEntries=" << data.v2.playlistEntries.size();
                }

                // Format 3 and newer support albums
                if(data.version >= 3) {
                    if(tableExists(dataAccess, "albums")) {
                        readUserData(dataAccess,
                                     "SELECT "
                                     " id,"
                                     " artistId,"
                                     " name "
                                     "FROM albums",
                                     data.v3.albums);
                    }
                    qDebug() << "Migration V3 read: albums=" << data.v3.albums.size();

                    if(tableExists(dataAccess, "albumEntries")) {
                        readUserData(dataAccess,
                                     "SELECT "
                                     " albumEntries.albumId,"
                                     " albumEntries.songId,"
                                     " albumEntries.trackNumber "
                                     "FROM albumEntries "
                                     " INNER JOIN songs ON songs.id=albumEntries.songId "
                                     " INNER JOIN trackers ON trackers.id=songs.tracker "
                                     " INNER JOIN formats ON formats.id=songs.format "
                                     " INNER JOIN artists ON artists.id=songs.artist "
                                     " INNER JOIN genres ON genres.id=songs.genre ",
                                     data.v3.albumEntries);
                    }
                    qDebug() << "Migration V3 read: albumEntries=" << data.v3.albumEntries.size();
                }

                // Format 4 and newer support radio, flags
            }
        }

        qDebug() << "Removing old database";
        QSqlDatabase::removeDatabase("catalog");
        QFile(catalogPath()).remove();
        qDebug() << "Removed old database";

        copyCatalogToDataFolder();

        qDebug().nospace()
                 << "Migration data: songs=" << data.v1.songs.size()
                 << ", user songs=" << data.v2.songs.size()
                 << ", artists=" << data.v2.artists.size()
                 << ", genres=" << data.v2.genres.size()
                 << ", playlists=" << data.v2.playlists.size()
                 << ", playlist entries=" << data.v2.playlistEntries.size()
                 << ", albums=" << data.v3.albums.size()
                 << ", album entries=" << data.v3.albumEntries.size();
        qDebug().space();

        if(!data.empty()) {
            using namespace bb::system;

            MessageBox message(tr("Updating Catalog"),
                               tr("Your song catalog is being updated. Please wait."));

            SqlDataAccess dataAccess(catalogPath(), "catalog");
            int totalProgress = data.totalUpdates();
            int currentProgress = 0;

            if(totalProgress <= 0) {
                totalProgress = 1;
            }

            // Migrate schema version 1 data
            if(!data.v1.songs.empty() &&
               dataAccess.connection().transaction()) {
                qDebug() << "Migration V1 write: songs=" << data.v1.songs.size();
                QString query = "UPDATE songs SET playCount=?, lastPlayed=?, myFavourite=? WHERE id=?";
                for(int i = 0; i < data.v1.songs.size(); ++i) {
                    dataAccess.execute(query, QVariantList() << data.v1.songs[i].playCount
                                                             << data.v1.songs[i].lastPlayed
                                                             << data.v1.songs[i].myFavourite
                                                             << data.v1.songs[i].id);
                    message.setProgress((++currentProgress) * 100 / totalProgress);
                }
                dataAccess.connection().commit();
            }


            // Migrate schema version 2 playlists
            if(!data.v2.playlists.empty() &&
               dataAccess.connection().transaction()) {
                qDebug() << "Migration V2 write: playlists=" << data.v2.playlists.size();
                QString query = "INSERT INTO playlists (id, name) VALUES (?,?)";
                for(int i = 0; i < data.v2.playlists.size(); ++i) {
                    dataAccess.execute(query, QVariantList() << data.v2.playlists[i].id
                                                             << data.v2.playlists[i].name);
                    message.setProgress((++currentProgress) * 100 / totalProgress);
                }
                dataAccess.connection().commit();
            }

            // Migrate schema version 2 artists
            if(!data.v2.artists.empty() &&
               dataAccess.connection().transaction()) {
                qDebug() << "Migration V2 write: artists=" << data.v2.artists.size();
                QString query = "INSERT INTO artists (id, name, score, downloads, rating) VALUES (?,?,0,0,0)";
                for(int i = 0; i < data.v2.artists.size(); ++i) {
                    dataAccess.execute(query, QVariantList() << data.v2.artists[i].id
                                                             << data.v2.artists[i].name);
                    message.setProgress((++currentProgress) * 100 / totalProgress);
                }
                dataAccess.connection().commit();
            }

            // Migrate schema version 2 genres
            if(!data.v2.genres.empty() &&
               dataAccess.connection().transaction()) {
                qDebug() << "Migration V2 write: genres=" << data.v2.genres.size();
                QString query = "INSERT INTO genres (id, name) VALUES (?,?)";
                for(int i = 0; i < data.v2.genres.size(); ++i) {
                    dataAccess.execute(query, QVariantList() << data.v2.genres[i].id
                                                             << data.v2.genres[i].name);
                    message.setProgress((++currentProgress) * 100 / totalProgress);
                }
                dataAccess.connection().commit();
            }

            // Migrate schema version 2 songs
            if(!data.v2.songs.empty() &&
               dataAccess.connection().transaction()) {
                qDebug() << "Migration V2 write: songs=" << data.v2.songs.size();
                QString query = "INSERT INTO songs "
                                    "(id,"
                                    " fileName,"
                                    " size,"
                                    " title,"
                                    " format,"
                                    " length,"
                                    " tracker,"
                                    " genre,"
                                    " artist,"
                                    " downloads,"
                                    " favourited,"
                                    " score,"
                                    " playCount,"
                                    " lastPlayed,"
                                    " myFavourite,"
                                    " patterns,"
                                    " orders,"
                                    " instruments,"
                                    " samples,"
                                    " channels)"
                                    " VALUES "
                                    "(?,?,?,?,?,?,?,?,?,?,"
                                    " ?,?,?,?,?,?,?,?,?,?)";
                for(int i = 0; i < data.v2.songs.size(); ++i) {
                    QVariantList params;
                    params << data.v2.songs[i].id
                           << data.v2.songs[i].fileName
                           << data.v2.songs[i].size
                           << data.v2.songs[i].title
                           << data.v2.songs[i].format
                           << data.v2.songs[i].length
                           << data.v2.songs[i].tracker
                           << data.v2.songs[i].genre
                           << data.v2.songs[i].artist
                           << data.v2.songs[i].downloads
                           << data.v2.songs[i].favourited
                           << data.v2.songs[i].score
                           << data.v2.songs[i].playCount
                           << data.v2.songs[i].lastPlayed
                           << data.v2.songs[i].myFavourite
                           << data.v2.songs[i].patterns
                           << data.v2.songs[i].orders
                           << data.v2.songs[i].instruments
                           << data.v2.songs[i].samples
                           << data.v2.songs[i].channels;
                    dataAccess.execute(query, params);
                    message.setProgress((++currentProgress) * 100 / totalProgress);
                }
                dataAccess.connection().commit();
            }

            // Migrate schema version 2 playlist entries
            if(!data.v2.playlistEntries.empty() &&
               dataAccess.connection().transaction()) {
                qDebug() << "Migration V2 write: playlistEntries=" << data.v2.playlistEntries.size();
                QString query = "INSERT INTO playlistEntries (playlistId, songId, songOrder) VALUES (?,?,?)";
                for(int i = 0; i < data.v2.playlistEntries.size(); ++i) {
                    dataAccess.execute(query, QVariantList() << data.v2.playlistEntries[i].playlistId
                                                             << data.v2.playlistEntries[i].songId
                                                             << data.v2.playlistEntries[i].songOrder);
                    message.setProgress((++currentProgress) * 100 / totalProgress);
                }
                dataAccess.connection().commit();
            }

            // Migrate schema version 3 albums
            if(!data.v3.albums.empty() &&
               dataAccess.connection().transaction()) {
                qDebug() << "Migration V3 write: albums=" << data.v3.albums.size();
                QString query = "INSERT INTO albums (id, artistId, name) VALUES (?,?,?)";
                for(int i = 0; i < data.v3.albums.size(); ++i) {
                    dataAccess.execute(query, QVariantList() << data.v3.albums[i].id
                                                             << data.v3.albums[i].artistId
                                                             << data.v3.albums[i].name);
                    message.setProgress((++currentProgress) * 100 / totalProgress);
                }
                dataAccess.connection().commit();
            }

            if(!data.v3.albumEntries.empty() &&
               dataAccess.connection().transaction()) {
                qDebug() << "Migration V3 write: albumEntries=" << data.v3.albumEntries.size();
                QString query = "INSERT INTO albumEntries (albumId, songId, trackNumber) VALUES (?,?,?)";
                for(int i = 0; i < data.v3.albumEntries.size(); ++i) {
                    dataAccess.execute(query, QVariantList() << data.v3.albumEntries[i].albumId
                                                             << data.v3.albumEntries[i].songId
                                                             << data.v3.albumEntries[i].trackNumber);
                    message.setProgress((++currentProgress) * 100 / totalProgress);
                }
                dataAccess.connection().commit();
            }

            message.setProgress(-1);

            qDebug() << "Migration done: optimizing...";
            message.setBody(tr("Optimizing catalog..."));

            dataAccess.execute("VACUUM");

            message.setProgress(100);

            qDebug() << "Migration completed successfully";
        } else {
            qDebug() << "No migration required";
        }

        QSqlDatabase::removeDatabase("catalog");
    }
}
