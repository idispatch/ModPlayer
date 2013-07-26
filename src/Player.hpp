#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <QObject>
#include <QVariant>

class Cache;
class Downloader;
class Unpacker;
class SongModule;

class Player : public QObject {
    Q_OBJECT
    Q_ENUMS(State)
public:
    enum State {
        Stopped,
        Preparing,
        Playing,
        Paused
    };

    Player(QObject * parent = 0);

    Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged FINAL)
    Q_PROPERTY(Cache* cache READ cache NOTIFY cacheChanged FINAL)
    Q_PROPERTY(Downloader* downloader READ downloader FINAL)
    Q_PROPERTY(SongModule* currentSong READ currentSong NOTIFY currentSongChanged FINAL)

    State state() const;
    QString statusText() const;

    Cache * cache() const;
    Downloader * downloader() const;
    SongModule * currentSong() const;

    Q_INVOKABLE void play(QVariant value); /* loads and plays */
    Q_INVOKABLE void stop(); /* stops but not unloads */

    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
Q_SIGNALS:
    void stateChanged();
    void statusTextChanged();
    void cacheChanged();
    void currentSongChanged();
private slots:
    void downloadStarted(int modId);
    void downloadFinished(QString fileName);
    void downloadFailure(int modId);
private:
    Q_DISABLE_COPY(Player)

    static QString fileNameOnly(QString const& fileName);

    void initCache();
    void initDownloader();
    void initModule();

    void changeStatus(QString const& text);
    void doPlay(QString const& fileName);

    void playByModuleId(int modId);
    void playByModuleFileName(QString const& fileName);

private:
    State m_state;
    QString m_statusText;
    Cache * m_cache;
    Downloader * m_downloader;
    Unpacker * m_unpacker;
    SongModule * m_module;
};

Q_DECLARE_METATYPE(Player*);

#endif
