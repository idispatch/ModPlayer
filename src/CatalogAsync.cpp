#include "Catalog.hpp"
#include <QDebug>

using namespace bb::data;
using namespace bb::cascades;

int Catalog::asyncCommandSubmit(Command * command) {
    QMutexLocker locker(&m_mutex);
    m_commandQueue.enqueue(command);
    m_cond.wakeOne();
    return command->id();
}

int Catalog::asyncCommand(Command::CommandType commandType) {
    return asyncCommandSubmit(new Command(commandType));
}

int Catalog::asyncSearchCommand(Command::CommandType commandType,
                                QString const& searchTerm,
                                int queryId,
                                int limit) {
    return asyncCommandSubmit(new SearchCommand(commandType,
                                                searchTerm,
                                                queryId,
                                                limit));
}

int Catalog::songCountAsync() {
    return asyncCommand(Command::SongCount);
}

int Catalog::personalSongCountAsync() {
    return asyncCommand(Command::PersonalSongCount);
}

int Catalog::findFormatsAsync() {
    return asyncCommand(Command::FormatsList);
}

int Catalog::findGenresAsync(QString const& searchTerm) {
    return asyncSearchCommand(Command::GenresList, searchTerm, 0, -1);
}

int Catalog::findArtistsAsync(QString const& searchTerm) {
    return asyncSearchCommand(Command::ArtistsList, searchTerm, 0, -1);
}

int Catalog::findPlaylistsAsync(QString const& searchTerm) {
    return asyncSearchCommand(Command::Playlists, searchTerm, 0, -1);
}

int Catalog::findAlbumsAsync(QString const& searchTerm) {
    return asyncSearchCommand(Command::Albums, searchTerm, 0, -1);
}

int Catalog::findSongsByFormatIdAsync(QString const& searchTerm, int formatId, int limit) {
    return asyncSearchCommand(Command::SongsByFormatList, searchTerm, formatId, limit);
}

int Catalog::findSongsByGenreIdAsync(QString const& searchTerm, int genreId, int limit) {
    return asyncSearchCommand(Command::SongsByGenreList, searchTerm, genreId, limit);
}

int Catalog::findSongsByArtistIdAsync(QString const& searchTerm, int artistId, int limit) {
    return asyncSearchCommand(Command::SongsByArtistList, searchTerm, artistId, limit);
}

int Catalog::findSongsByPlaylistIdAsync(QString const& searchTerm, int playlistId, int limit) {
    return asyncSearchCommand(Command::SongsByPlaylist, searchTerm, playlistId, limit);
}

int Catalog::findSongsByAlbumIdAsync(QString const& searchTerm, int albumId, int limit) {
    return asyncSearchCommand(Command::SongsByAlbum, searchTerm, albumId, limit);
}

int Catalog::findSongsBySongAlbumIdAsync(QString const& searchTerm, int songId, int limit) {
    return asyncSearchCommand(Command::SongsBySongAlbum, searchTerm, songId, limit);
}

int Catalog::findMostDownloadedSongsAsync(QString const& searchTerm, int limit) {
    return asyncSearchCommand(Command::MostDownloadedSongs, searchTerm, 0, limit);
}

int Catalog::findMostFavouritedSongsAsync(QString const& searchTerm, int limit) {
    return asyncSearchCommand(Command::MostFavouritedSongs, searchTerm, 0, limit);
}

int Catalog::findMostScoredSongsAsync(QString const& searchTerm, int limit) {
    return asyncSearchCommand(Command::MostScoredSongs, searchTerm, 0, limit);
}

int Catalog::findRecentlyPlayedSongsAsync(QString const& searchTerm, int limit) {
    return asyncSearchCommand(Command::RecentlyPlayedSongs, searchTerm, 0, limit);
}

int Catalog::findMyFavouriteSongsAsync(QString const& searchTerm, int limit) {
    return asyncSearchCommand(Command::MyFavouriteSongs, searchTerm, 0, limit);
}

int Catalog::findMyLocalSongsAsync(QString const& searchTerm, int limit) {
    return asyncSearchCommand(Command::MyLocalSongs, searchTerm, 0, limit);
}

int Catalog::findMostPlayedSongsAsync(QString const& searchTerm, int limit) {
    return asyncSearchCommand(Command::MostPlayedSongs, searchTerm, 0, limit);
}

int Catalog::findLiveStreamRadioAsync(QString const& searchTerm,
                                      QString const& country,
                                      int limit) {
    return asyncCommandSubmit(new SearchRadioCommand(Command::Radio,
                                                     searchTerm,
                                                     country,
                                                     Command::nextCommandId(),
                                                     limit));
}

int Catalog::searchSongsAsync(QString const& searchTerm, int limit) {
    return asyncSearchCommand(Command::SearchSongs, searchTerm, 0, limit);
}

void Catalog::run() {
    bool exitRequested = false;
    while(!exitRequested)
    {
        m_mutex.lock();
        while(m_commandQueue.empty())
        {
            m_cond.wait(&m_mutex);
        }

        std::auto_ptr<Command> command(m_commandQueue.dequeue());
        m_mutex.unlock();

        QVariant result;
        switch(command->command())
        {
        case Command::ExitCommand:
            exitRequested = true;
            m_mutex.lock();
            while(!m_commandQueue.empty())
            {
                delete m_commandQueue.dequeue();
            }
            m_mutex.unlock();
            break;
        case Command::SongCount:
            result = QVariant::fromValue(songCount());
            break;
        case Command::PersonalSongCount:
            result = QVariant::fromValue(personalSongCount());
            break;
        case Command::SearchSongs:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, searchSongs(searchCommand->query(),
                                                     searchCommand->limit()));
            }
            break;
        case Command::FormatsList:
            result = assign(command, findFormats());
            break;
        case Command::GenresList:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findGenres(searchCommand->query()));
            }
            break;
        case Command::ArtistsList:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findArtists(searchCommand->query()));
            }
            break;
        case Command::Playlists:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findPlaylists(searchCommand->query()));
            }
            break;
        case Command::Albums:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findAlbums(searchCommand->query()));
            }
            break;
        case Command::Radio:
            {
                SearchRadioCommand * searchCommand = dynamic_cast<SearchRadioCommand*>(command.get());
                result = assign(command, findLiveStreamRadio(searchCommand->query(),
                                                             searchCommand->country(),
                                                             searchCommand->limit()));
            }
            break;
        case Command::SongsByFormatList:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findSongsByFormatId(searchCommand->query(),
                                                             searchCommand->queryId(),
                                                             searchCommand->limit()));
            }
            break;
        case Command::SongsByArtistList:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findSongsByArtistId(searchCommand->query(),
                                                             searchCommand->queryId(),
                                                             searchCommand->limit()));
            }
            break;
        case Command::SongsByGenreList:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findSongsByGenreId(searchCommand->query(),
                                                            searchCommand->queryId(),
                                                            searchCommand->limit()));
            }
            break;
        case Command::SongsByPlaylist:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findSongsByPlaylistId(searchCommand->query(),
                                                               searchCommand->queryId(),
                                                               searchCommand->limit()));
            }
            break;
        case Command::SongsByAlbum:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findSongsByAlbumId(searchCommand->query(),
                                                            searchCommand->queryId(),
                                                            searchCommand->limit()));
            }
            break;
        case Command::SongsBySongAlbum:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findSongsBySongAlbumId(searchCommand->query(),
                                                                searchCommand->queryId(),
                                                                searchCommand->limit()));
            }
            break;
        case Command::MostDownloadedSongs:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findMostDownloadedSongs(searchCommand->query(),
                                                                 searchCommand->limit()));
            }
            break;
        case Command::MostFavouritedSongs:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findMostFavouritedSongs(searchCommand->query(),
                                                                 searchCommand->limit()));
            }
            break;
        case Command::MostScoredSongs:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findMostScoredSongs(searchCommand->query(),
                                                             searchCommand->limit()));
            }
            break;
        case Command::RecentlyPlayedSongs:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findRecentlyPlayedSongs(searchCommand->query(),
                                                                 searchCommand->limit()));
            }
            break;
        case Command::MyFavouriteSongs:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findMyFavouriteSongs(searchCommand->query(),
                                                              searchCommand->limit()));
            }
            break;
        case Command::MyLocalSongs:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findMyLocalSongs(searchCommand->query(),
                                                          searchCommand->limit()));
            }
            break;
        case Command::MostPlayedSongs:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findMostPlayedSongs(searchCommand->query(),
                                                             searchCommand->limit()));
            }
            break;
        default:
            qDebug() << "Unknown command:" << command->command();
            break;
        }

        emit resultReady(command->id(), result);

        QObject* p = result.value<QObject*>();
        if(p != NULL && p->parent() == NULL)
        {
            result.detach();
            delete p;
        }
    }
    QThread::exit(0);
}
