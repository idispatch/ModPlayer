#ifndef PLAYBACK_HPP_
#define PLAYBACK_HPP_

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QByteArray>
#include <QSettings>
#include <bb/multimedia/MediaPlayer>
#include <sys/asoundlib.h>
#include "SongModule.hpp"
#include "PlaybackConfig.hpp"
#include "InstanceCounter.hpp"

class Playback : public QThread,
                 public InstanceCounter<Playback> {
    Q_OBJECT
    Q_PROPERTY(SongModule* currentSong READ currentSong NOTIFY currentSongChanged FINAL)
    Q_PROPERTY(PlaybackConfig* configuration READ configuration NOTIFY configurationChanged FINAL)
public:
    enum State
    {
        Idle = 0,      /* no module loaded and not playing, thread loop is running */
        Loaded = 1,    /* module loaded and stopped */
        Playing = 2,   /* module loaded and playing */
        Paused = 3,    /* module loaded and paused */
        Exiting = 4,   /* currently exiting */
        Exit = 5       /* no module loaded and thread loop exited */
    };

    Playback(QSettings &settings, QObject * parent);
    virtual ~Playback();

    State state();
    SongModule* currentSong();
    PlaybackConfig* configuration();

    void stopThread();

    Q_INVOKABLE void configure();

    Q_INVOKABLE bool load(SongExtendedInfo const& info, QString const& fileName);
    Q_INVOKABLE bool unload();

    Q_INVOKABLE bool play();
    Q_INVOKABLE bool stop();
    Q_INVOKABLE bool pause();
    Q_INVOKABLE bool resume();
    Q_INVOKABLE bool rewind();
    Q_INVOKABLE bool seek(int position);

    using InstanceCounter<Playback>::getInstanceCount;
    using InstanceCounter<Playback>::getMaxInstanceCount;
Q_SIGNALS:
    void currentSongChanged(); // will never be emitted because song is created once
    void configurationChanged(); // will never be emitted because song is created once

    /* Playback state changes */
    void playing();
    void stopped();
    void songFinished();
    void paused();
    void metaDataChanged();
    void bufferingLevelChanged(double level);
    void bufferingStatusChanged(int type);

private slots:
    void onMediaPlayerBufferLevelChanged(double level);
    void onMediaPlayerBufferStatusChanged(bb::multimedia::BufferStatus::Type type);
    void onMediaPlayerError(bb::multimedia::MediaError::Type mediaError, unsigned int position);
    void onMediaPlayerMediaStateChanged(bb::multimedia::MediaState::Type type);
    void onMediaPlayerMetaDataChanged(const QVariantMap &metaData);
    void onMediaPlayerPlaybackCompleted();
    void onMediaPlayerPositionChanged(unsigned int);
private:
    Q_DISABLE_COPY(Playback)
    void run();
    bool detectAudioDevice();
    bool initPlayback();
    int  updateChunk();
    void stopAudioDevice();
    void closePlayback();
    void changeState(State state);
private:
    enum Command
    {
        NoCommand = 0,
        ExitCommand,
        ConfigureCommand,
        LoadCommand,
        UnloadCommand,
        PlayCommand,
        StopCommand,
        PauseCommand,
        ResumeCommand,
        RewindCommand,
        SeekCommand
    };

    bool submitCommadAndWait(Command);
    void initMediaPlayer();
    void loadSettings();
    void saveSettings();
    void configureAudio();
    void initAudioManager();

    PlaybackConfig m_config;

    QSettings &m_settings;
    QMutex m_mutex;
    QWaitCondition m_cond;
    State m_state;
    Command m_command;
    QString m_pendingFileName;
    SongExtendedInfo m_pendingSong;
    int m_pendingPosition;

    QByteArray m_audioBuffer;
    SongModule m_song;
    bb::multimedia::MediaPlayer * m_mediaPlayer;

    /* Device audio subsystem */
    int m_numDevices;
    int m_pcmFd;
    unsigned int m_audioman_handle;
    snd_pcm_t * m_playback_handle;
};

Q_DECLARE_METATYPE(Playback*);

#endif
