#include "FileUtils.hpp"
#include "Catalog.hpp"
#include "Analytics.hpp"
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

void Catalog::initCatalog() {
    copyCatalogToDataFolder();
    m_dataAccess = new SqlDataAccess(catalogPath(), "catalog", this);
}

QString Catalog::catalogPath() const {
    return FileUtils::joinPath(QDir::homePath(), "catalog.sqlite");
}

void Catalog::copyCatalogToDataFolder() {
    QFile catalogFile(catalogPath());
    // If the catalog does not exist in app sandbox then the app was started the first time
    // Copy the initial catalog to the app sandbox
    if(!catalogFile.exists()) {
        qDebug() << "Personal catalog does not exist at" << catalogFile.fileName();

        MessageBox message(tr("Initializing Catalog"),
                           tr("Your song catalog is being initialized. Please wait."));

        QString appFolder(QDir::homePath());
        appFolder.chop(4);
        QString originalFileName = appFolder + "app/native/assets/catalog.sqlite";
        QFile originalFile(originalFileName);

        if(originalFile.exists()) {
            qDebug() << "Original catalog is at" << originalFile.fileName();

            if(!originalFile.copy(catalogFile.fileName())) {
                qDebug() << "Failed to copy catalog to path: " << catalogFile.fileName();
            }
        } else {
            qDebug() << "Failed to copy catalog - file does not exists";
        }
    } else {
        // Catalog exists. Check the catalog version and perform upgrade if required.
        typedef CatalogMigrationData::Catalog MigrationData;
        MigrationData data;
        {
            SqlDataAccess dataAccess(catalogPath(), "catalog");
            {
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
                qDebug() << "Starting database migration from version" << data.version << "to version" << Catalog::Version;

                MessageBox message(tr("Updating Catalog"),
                                   tr("Your song catalog is being updated. Please wait."));

                if(data.version >= 0) {
                    // All formats support play count, last played, my favourite
                    const char * query = "SELECT id, playCount, lastPlayed, myFavourite "
                                         "FROM songs "
                                         "WHERE playCount>0 OR lastPlayed>0 OR myFavourite>0";
                    readUserData(dataAccess, query, data.v1.songs);
                }

                if(data.version >= 2) {
                    // Format 2 and newer support user songs
                    const char * query = "SELECT"
                                         " id,"
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
                                         " channels "
                                         "FROM songs WHERE id < 0";
                    readUserData(dataAccess, query, data.v2.songs);
                }
                if(data.version >= 2) {
                    // Format 2 and newer support playlists
                    readUserData(dataAccess,
                                 "SELECT id, name FROM playlists",
                                 data.v2.playlists);
                    readUserData(dataAccess,
                                 "SELECT playlistId, songId, songOrder FROM playlistEntries",
                                 data.v2.playlistEntries);
                }
            }
        }

        QSqlDatabase::removeDatabase("catalog");

        qDebug() << "Removing old database";
        catalogFile.remove();
        qDebug() << "Removed old database";

        copyCatalogToDataFolder();

        qDebug().nospace()
                 << "Migration data: songs="
                 << data.v1.songs.size()
                 << ", user songs="
                 << data.v2.songs.size()
                 << ", playlists="
                 << data.v2.playlists.size()
                 << ", playlist entries="
                 << data.v2.playlistEntries.size();
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
            for(int i = 0; i < data.v1.songs.size(); ++i) {
                QString query = "UPDATE songs SET playCount=?, lastPlayed=?, myFavourite=? WHERE id=?";
                QVariantList params;
                params << data.v1.songs[i].playCount
                       << data.v1.songs[i].lastPlayed
                       << data.v1.songs[i].myFavourite
                       << data.v1.songs[i].id;
                dataAccess.execute(query, params);
                message.setProgress((++currentProgress) * 100 / totalProgress);
            }

            // Migrate schema version 2 data
            for(int i = 0; i < data.v2.playlists.size(); ++i) {
                QString query = "INSERT INTO playlists (id, name) VALUES (?,?)";
                QVariantList params;
                params << data.v2.playlists[i].id
                       << data.v2.playlists[i].name;
                dataAccess.execute(query, params);
                message.setProgress((++currentProgress) * 100 / totalProgress);
            }

            for(int i = 0; i < data.v2.songs.size(); ++i) {
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

            for(int i = 0; i < data.v2.playlistEntries.size(); ++i) {
                QString query = "INSERT INTO playlistEntries (playlistId, songId, songOrder) VALUES (?,?,?)";
                QVariantList params;
                params << data.v2.playlistEntries[i].playlistId
                       << data.v2.playlistEntries[i].songId
                       << data.v2.playlistEntries[i].songOrder;
                dataAccess.execute(query, params);
                message.setProgress((++currentProgress) * 100 / totalProgress);
            }

            message.setProgress(-1);
            message.setBody(tr("Optimizing catalog..."));

            dataAccess.execute("VACUUM");

            message.setProgress(100);

            qDebug() << "Migration completed successfully";
#if 0
            message.setBody(tr("Your song catalog has been successfully updated.")).enableButton(true).run();
#endif
        } else {
            qDebug() << "No migration required";
        }

        QSqlDatabase::removeDatabase("catalog");
    }
}
