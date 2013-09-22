#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QUrl>
#include <QSettings>
#include "InstanceCounter.hpp"

class Cache;
class Catalog;
class Downloader;
class Unpacker;
class SongModule;
class ModPlayback;

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

    Q_PROPERTY(Cache* cache READ cache NOTIFY cacheChanged FINAL)
    Q_PROPERTY(Catalog* catalog READ catalog NOTIFY catalogChanged FINAL)
    Q_PROPERTY(Downloader* downloader READ downloader FINAL)
    Q_PROPERTY(ModPlayback* playback READ playback NOTIFY playbackChanged FINAL)
    Q_PROPERTY(SongModule* currentSong READ currentSong NOTIFY currentSongChanged FINAL)

    State state() const;
    QString statusText() const;

    Cache * cache() const;
    Catalog * catalog() const;
    Downloader * downloader() const;
    ModPlayback * playback() const;
    SongModule * currentSong() const;

    Q_INVOKABLE void load();
    Q_INVOKABLE void play(QVariant value); /* loads and plays */
    Q_INVOKABLE void stop(); /* stops but not unloads */
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();

    using InstanceCounter<Player>::getInstanceCount;
    using InstanceCounter<Player>::getMaxInstanceCount;
Q_SIGNALS:
    void stateChanged();
    void statusTextChanged();
    void cacheChanged();
    void catalogChanged();
    void playbackChanged();
    void currentSongChanged();
    void localSongLoaded();
private slots:
    /* For FilePicker */
    void onSongLoadSelected(const QStringList&);
    void onSongLoadCanceled();

    /* For Downloader */
    void onDownloadStarted(int id);
    void onDownloadFinished(QString fileName);
    void onDownloadFailure(int id);

    /* For ModPlayback */
    void onPaused();
    void onPlaying();
    void onStopped();

    /* For NowPlayingConnection */
    void onNowPlayingAcquired();
    void onNowPlayingRevoked();
    void onNowPlayingPlay();
    void onNowPlayingStop();
    void onNowPlayingPause();
private:
    Q_DISABLE_COPY(Player)

    static QString joinPath(QString const& directory, QString const& fileName);
    static QString fileNameOnly(QString const& fileName);

    void initCache();
    void initDownloader();
    void initPlayback();
    void initNowPlaying();

    void changeStatus(State state, QString const& statusText);
    void beginPlay(bool fromCatalog, QString const& fileName);

    void playByModuleId(int id);
    void playByModuleFileName(QString const& fileName);

    void updateNowPlaying();
private:
    QSettings &m_settings;
    State m_state;
    QMap<int, QUrl> m_formatIdToIconUrlMap;
    QString m_statusText;
    Catalog * m_catalog;
    Cache * m_cache;
    Downloader * m_downloader;
    Unpacker * m_unpacker;
    ModPlayback * m_playback;
    bb::multimedia::NowPlayingConnection * m_nowPlaying;
};

Q_DECLARE_METATYPE(Player*);

#endif
