#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QUrl>
#include <QSettings>
#include <QStringList>
#include "InstanceCounter.hpp"

class Cache;
class Catalog;
class Downloader;
class InternetRadio;
class Unpacker;
class SongModule;
class Playback;
class Playlist;
class Importer;
class SuspendPlayback;

namespace bb {
    namespace multimedia {
        class NowPlayingConnection;
    }
}

class Player : public QObject,
               public InstanceCounter<Player> {
    Q_OBJECT
    Q_ENUMS(State)
public:
    enum State
    {
        Stopped = 0,
        Playing = 1,
        Paused = 2,
        Resolving = 100,
        Downloading = 101,
        Preparing = 102
    };

    Player(QSettings &settings, QObject * parent);
    ~Player();

    Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged FINAL)
    Q_PROPERTY(QString userDirectory READ userDirectory WRITE setUserDirectory NOTIFY userDirectoryChanged FINAL)

    Q_PROPERTY(Cache* cache READ cache NOTIFY cacheChanged FINAL)
    Q_PROPERTY(Catalog* catalog READ catalog NOTIFY catalogChanged FINAL)
    Q_PROPERTY(Downloader* downloader READ downloader FINAL)
    Q_PROPERTY(InternetRadio* radio READ radio FINAL)
    Q_PROPERTY(Playback* playback READ playback NOTIFY playbackChanged FINAL)
    Q_PROPERTY(Playlist* playlist READ playlist NOTIFY playlistChanged FINAL)
    Q_PROPERTY(SongModule* currentSong READ currentSong NOTIFY currentSongChanged FINAL)

    State state() const;
    QString statusText() const;

    QString userDirectory() const;
    void setUserDirectory(QString const& value);

    Cache * cache() const;
    Catalog * catalog() const;
    Downloader * downloader() const;
    InternetRadio * radio() const;
    Playback * playback() const;
    Playlist * playlist() const;
    SongModule * currentSong() const;

    Q_INVOKABLE void playPlaylist();
    Q_INVOKABLE void playLocalSong(QString const& fileName);
    Q_INVOKABLE void browseForLocalSong();
    Q_INVOKABLE void play(QVariant value); /* loads and plays */
    Q_INVOKABLE void stop(); /* stops but not unloads */
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void seek(int position);
    Q_INVOKABLE void playNext();
    Q_INVOKABLE void playPrevious();

    Q_INVOKABLE void exportMp3(QString const& inputFileName,
                               QString const& outputFileName);
    Q_INVOKABLE void importSongs();

    using InstanceCounter<Player>::getInstanceCount;
    using InstanceCounter<Player>::getMaxInstanceCount;
Q_SIGNALS:
    void requestPlayerView();
    void stateChanged();
    void statusTextChanged();
    void userDirectoryChanged();
    void cacheChanged();
    void catalogChanged();
    void playbackChanged();
    void playlistChanged();
    void currentSongChanged();
private slots:
    /* For FilePicker */
    void onLocalSongSelected(const QStringList&);
    void onLocalSongBrowseCanceled();

    /* For Cache */
    void onCurrentFilesChanged();

    /* For Downloader */
    void onDownloadStarted(int id);
    void onDownloadFinished(QString fileName);
    void onDownloadFailure(int id);

    /* For Playback */
    void onPaused();
    void onPlaying();
    void onStopped();
    void onFinished();

    /* For NowPlayingConnection */
    void onNowPlayingAcquired();
    void onNowPlayingRevoked();
    void onNowPlayingPlay();
    void onNowPlayingStop();
    void onNowPlayingPause();
    void onNowPlayingNext();
    void onNowPlayingPrevious();

    /* For Playlist */
    void onPlaylistNextAvailableChanged();
    void onPlaylistPreviousAvailableChanged();

    /* For Importer */
    void onImportSongsCompleted();
protected:
    void timerEvent(QTimerEvent *event);
private:
    Q_DISABLE_COPY(Player)

    void initCatalog();
    void initCache();
    void initDownloader();
    void initRadio();
    void initPlayback();
    void initNowPlaying();
    void initPlaylist();

    void changeStatus(State state, QString const& statusText);
    bool beginPlay(bool fromCatalog, QString const& fileName);

    void playByModuleId(int id);
    void playByModuleFileName(QString const& fileName);

    void updateNowPlaying();

    void askToSupport();
    void askToImport();
private:
    int m_feedbackTimerId;
    int m_importTimerId;
    QStringList m_fileNameFilters;
    QSettings &m_settings;
    State m_state;
    QMap<int, QUrl> m_formatIdToIconUrlMap;
    QString m_statusText;
    QString m_userDirectory;
    Catalog * m_catalog;
    Cache * m_cache;
    Downloader * m_downloader;
    InternetRadio * m_internetRadio;
    Unpacker * m_unpacker;
    Playback * m_playback;
    Playlist * m_playlist;
    Importer * m_importer;
    std::auto_ptr<SuspendPlayback> m_playBackSuspend;
    bb::multimedia::NowPlayingConnection * m_nowPlaying;
};

Q_DECLARE_METATYPE(Player*);

#endif
