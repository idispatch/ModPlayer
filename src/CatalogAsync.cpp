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

int Catalog::findArtistsAsync() {
    return asyncCommand(Command::ArtistsList);
}

int Catalog::findPlaylistsAsync() {
    return asyncCommand(Command::Playlists);
}

int Catalog::findAlbumsAsync() {
    return asyncCommand(Command::Albums);
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
                ArrayDataModel * model = searchSongs(searchCommand->query(), searchCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::FormatsList:
            {
                ArrayDataModel * model = findFormats();
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::GenresList:
            {
                GroupDataModel * model = findGenres();
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::ArtistsList:
            {
                GroupDataModel * model = findArtists();
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::Playlists:
            {
                GroupDataModel * model = findPlaylists();
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::Albums:
            {
                GroupDataModel * model = findAlbums();
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByFormatList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findSongsByFormatId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByArtistList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findSongsByArtistId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByGenreList:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findSongsByGenreId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByPlaylist:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findSongsByPlaylistId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::SongsByAlbum:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findSongsByAlbumId(findCommand->queryId(), findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostDownloadedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMostDownloadedSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostFavouritedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMostFavouritedSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostScoredSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMostScoredSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::RecentlyPlayedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findRecentlyPlayedSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MyFavouriteSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMyFavouriteSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MyLocalSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMyLocalSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
            }
            break;
        case Command::MostPlayedSongs:
            {
                FindCommand * findCommand = dynamic_cast<FindCommand*>(command.get());
                ArrayDataModel * model = findMostPlayedSongs(findCommand->limit());
                model->moveToThread(command->thread());
                result = QVariant::fromValue(model);
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
