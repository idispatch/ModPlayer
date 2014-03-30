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

int Catalog::asyncFindCommand(Command::CommandType commandType, int id, int limit) {
    return asyncCommandSubmit(new FindCommand(commandType, id, limit));
}

int Catalog::asyncSearchCommand(QString const& query, int limit) {
    return asyncCommandSubmit(new SearchCommand(query, limit));
}

int Catalog::asyncSearchCommand(Command::CommandType commandType, QString const& query, int limit) {
    return asyncCommandSubmit(new SearchCommand(commandType, query, limit));
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

int Catalog::findGenresAsync() {
    return asyncCommand(Command::GenresList);
}

int Catalog::findArtistsAsync(QString const& searchTerm) {
    return asyncSearchCommand(Command::ArtistsList, searchTerm, -1);
}

int Catalog::findPlaylistsAsync() {
    return asyncCommand(Command::Playlists);
}

int Catalog::findAlbumsAsync(QString const& searchTerm) {
    return asyncSearchCommand(Command::Albums, searchTerm, -1);
}

int Catalog::findSongsByFormatIdAsync(int formatId, int limit) {
    return asyncFindCommand(Command::SongsByFormatList, formatId, limit);
}

int Catalog::findSongsByGenreIdAsync(int genreId, int limit) {
    return asyncFindCommand(Command::SongsByGenreList, genreId, limit);
}

int Catalog::findSongsByArtistIdAsync(int artistId, int limit) {
    return asyncFindCommand(Command::SongsByArtistList, artistId, limit);
}

int Catalog::findSongsByPlaylistIdAsync(int playlistId, int limit) {
    return asyncFindCommand(Command::SongsByPlaylist, playlistId, limit);
}

int Catalog::findSongsByAlbumIdAsync(int albumId, int limit) {
    return asyncFindCommand(Command::SongsByAlbum, albumId, limit);
}

int Catalog::findMostDownloadedSongsAsync(int limit) {
    return asyncFindCommand(Command::MostDownloadedSongs, 0, limit);
}

int Catalog::findMostFavouritedSongsAsync(int limit) {
    return asyncFindCommand(Command::MostFavouritedSongs, 0, limit);
}

int Catalog::findMostScoredSongsAsync(int limit) {
    return asyncFindCommand(Command::MostScoredSongs, 0, limit);
}

int Catalog::findRecentlyPlayedSongsAsync(int limit) {
    return asyncFindCommand(Command::RecentlyPlayedSongs, 0, limit);
}

int Catalog::findMyFavouriteSongsAsync(int limit) {
    return asyncFindCommand(Command::MyFavouriteSongs, 0, limit);
}

int Catalog::findMyLocalSongsAsync(int limit) {
    return asyncFindCommand(Command::MyLocalSongs, 0, limit);
}

int Catalog::findMostPlayedSongsAsync(int limit) {
    return asyncFindCommand(Command::MostPlayedSongs, 0, limit);
}

int Catalog::searchSongsAsync(QString const& searchTerm, int limit) {
    return asyncSearchCommand(searchTerm, limit);
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
            result = assign(command, findGenres());
            break;
        case Command::ArtistsList:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findArtists(searchCommand->query()));
            }
            break;
        case Command::Playlists:
            result = assign(command, findPlaylists());
            break;
        case Command::Albums:
            {
                SearchCommand * searchCommand = dynamic_cast<SearchCommand*>(command.get());
                result = assign(command, findAlbums(searchCommand->query()));
            }
            break;
        case Command::SongsByFormatList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findSongsByFormatId(findCommand->queryId(),
                                                             findCommand->limit()));
            }
            break;
        case Command::SongsByArtistList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findSongsByArtistId(findCommand->queryId(),
                                                             findCommand->limit()));
            }
            break;
        case Command::SongsByGenreList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findSongsByGenreId(findCommand->queryId(),
                                                            findCommand->limit()));
            }
            break;
        case Command::SongsByPlaylist:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findSongsByPlaylistId(findCommand->queryId(),
                                                               findCommand->limit()));
            }
            break;
        case Command::SongsByAlbum:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findSongsByAlbumId(findCommand->queryId(),
                                                            findCommand->limit()));
            }
            break;
        case Command::MostDownloadedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findMostDownloadedSongs(findCommand->limit()));
            }
            break;
        case Command::MostFavouritedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findMostFavouritedSongs(findCommand->limit()));
            }
            break;
        case Command::MostScoredSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findMostScoredSongs(findCommand->limit()));
            }
            break;
        case Command::RecentlyPlayedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findRecentlyPlayedSongs(findCommand->limit()));
            }
            break;
        case Command::MyFavouriteSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findMyFavouriteSongs(findCommand->limit()));
            }
            break;
        case Command::MyLocalSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findMyLocalSongs(findCommand->limit()));
            }
            break;
        case Command::MostPlayedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                result = assign(command, findMostPlayedSongs(findCommand->limit()));
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
