#ifndef MODPLAYBACK_HPP_
#define MODPLAYBACK_HPP_

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QByteArray>
#include <sys/asoundlib.h>
#include "SongModule.hpp"
#include "PlaybackConfig.hpp"

class ModPlayback : public QThread {
    Q_OBJECT
    Q_PROPERTY(SongModule* currentSong READ currentSong NOTIFY currentSongChanged FINAL)
public:
    enum State
    {
        Idle,      /* no module loaded and not playing, thread loop is running */
        Loaded,    /* module loaded and stopped */
        Playing,   /* module loaded and playing */
        Paused,    /* module loaded and paused */
        Exiting,   /* currently exiting */
        Exit       /* no module loaded and thread loop exited */
    };

    ModPlayback(QObject * parent);
    virtual ~ModPlayback();

    State state();
    SongModule* currentSong();

    void run();
    void stopThread();

    Q_INVOKABLE void configure(QVariant const& setup);

    Q_INVOKABLE bool load(SongInfo const& info, QString const& fileName);
    Q_INVOKABLE bool unload();

    Q_INVOKABLE bool play();
    Q_INVOKABLE bool stop();
    Q_INVOKABLE bool pause();
    Q_INVOKABLE bool resume();
    Q_INVOKABLE bool rewind();
Q_SIGNALS:
    void currentSongChanged(); // will never be emitted because song is created once

    /* Playback state changes */
    void playing();
    void stopped();
    void paused();
private:
    Q_DISABLE_COPY(ModPlayback)

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
        RewindCommand
    };

    PlaybackConfig m_config;
    PlaybackConfig m_pendingConfig;

    QMutex m_mutex;
    QWaitCondition m_cond;
    State m_state;
    Command m_command;
    QString m_pendingFileName;
    SongInfo m_pendingSong;

    QByteArray m_audioBuffer;
    SongModule m_song;

    /* Device audio channel configuration */
    bool m_bStereo;
    int m_frequency;
    int m_sampleBitSize;

    /* Device audio subsystem */
    int m_numDevices;
    int m_pcmFd;
    snd_pcm_t * m_playback_handle;
};

Q_DECLARE_METATYPE(ModPlayback*);

#endif
