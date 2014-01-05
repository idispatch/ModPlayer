#include "Playback.hpp"

#include <string.h>
#include <errno.h>
#include <QDebug>
#include <QVector>

#include "modplug.h"
#include "Analytics.hpp"
#include "ApplicationUI.hpp"

template<>
int InstanceCounter<Playback>::s_count;
template<>
int InstanceCounter<Playback>::s_maxCount;

#ifdef _DEBUG
#   define VERBOSE_LOGGING
//# define PERFORMANCE_MEASURE
#   ifdef PERFORMANCE_MEASURE
static _Uint64t get_clock()
{
    struct timespec t;
    if(clock_gettime(CLOCK_MONOTONIC, &t) == 0)
    {
        return timespec2nsec(&t);
    }
    return 0;
}
#   endif
#else
#endif

Playback::Playback(QSettings &settings, QObject * parent)
    : QThread(parent),
      m_config(NULL),
      m_settings(settings),
      m_mutex(QMutex::NonRecursive),
      m_state(Exit),
      m_command(NoCommand),
      m_pendingSong(NULL),
      m_song(NULL),
      m_mediaPlayer(new bb::multimedia::MediaPlayer(this)),
      m_numDevices(0),
      m_pcmFd(-1),
      m_playback_handle(NULL) {
    initMediaPlayer();
    loadSettings();
}

Playback::~Playback() {
    saveSettings();
    closePlayback(); // called from playback thread only
}

void Playback::initMediaPlayer() {
    bool rc;
    Q_UNUSED(rc);
    bb::multimedia::MediaError::Type mediaError;

    if(m_mediaPlayer->audioOutput() != bb::multimedia::AudioOutput::Default) {
        mediaError = m_mediaPlayer->setAudioOutput(bb::multimedia::AudioOutput::Default);
        if(mediaError != bb::multimedia::MediaError::None) {
            qDebug() << "initMediaPlayer" << "setAudioOutput:" << mediaError;
        }
    }

    mediaError = m_mediaPlayer->setStatusInterval(200);
    if(mediaError != bb::multimedia::MediaError::None) {
        qDebug() << "initMediaPlayer" << "setStatusInterval:" << mediaError;
    }

    if(m_mediaPlayer->speed() != 1.0) {
        mediaError = m_mediaPlayer->setSpeed(1.0);
        if(mediaError != bb::multimedia::MediaError::None) {
            qDebug() << "initMediaPlayer" << "setSpeed:" << mediaError;
        }
    }

    if(m_mediaPlayer->repeatMode() != bb::multimedia::RepeatMode::None) {
        mediaError = m_mediaPlayer->setRepeatMode(bb::multimedia::RepeatMode::None);
        if(mediaError != bb::multimedia::MediaError::None) {
            qDebug() << "initMediaPlayer" << "setRepeatMode:" << mediaError;
        }
    }

    rc = QObject::connect(m_mediaPlayer, SIGNAL(bufferStatusChanged(bb::multimedia::BufferStatus::Type)),
                          this,          SLOT(onMediaPlayerBufferStatusChanged(bb::multimedia::BufferStatus::Type)));
    Q_ASSERT(rc);

    rc = QObject::connect(m_mediaPlayer, SIGNAL(error(bb::multimedia::MediaError::Type, unsigned int)),
                          this,          SLOT(onMediaPlayerError(bb::multimedia::MediaError::Type, unsigned int)));
    Q_ASSERT(rc);

    rc = QObject::connect(m_mediaPlayer, SIGNAL(mediaStateChanged(bb::multimedia::MediaState::Type)),
                          this,          SLOT(onMediaPlayerMediaStateChanged(bb::multimedia::MediaState::Type)));
    Q_ASSERT(rc);

    rc = QObject::connect(m_mediaPlayer, SIGNAL(metaDataChanged(const QVariantMap&)),
                          this,          SLOT(onMediaPlayerMetaDataChanged(const QVariantMap&)));
    Q_ASSERT(rc);

    rc = QObject::connect(m_mediaPlayer, SIGNAL(playbackCompleted()),
                          this,          SLOT(onMediaPlayerPlaybackCompleted()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_mediaPlayer, SIGNAL(positionChanged(unsigned int)),
                          this,          SLOT(onMediaPlayerPositionChanged(unsigned int)));
    Q_ASSERT(rc);
}

void Playback::loadSettings() {
    m_settings.beginGroup("mixer");

    m_config.setStereo(m_settings.value("stereo", true).toBool());
    m_config.setSampleSize(m_settings.value("sampleSize", 16).toInt());
    m_config.setFrequency(m_settings.value("frequency", 44100).toInt());

    m_config.setResamplingMode(m_settings.value("resamplingMode", 3).toInt());
    m_config.setStereoSeparation(m_settings.value("stereoSeparation", 128).toInt());
    m_config.setMaximumMixingChannels(m_settings.value("mixingChannels", 128).toInt());

    m_config.setReverbEnabled(m_settings.value("reverbEnabled", true).toBool());
    m_config.setReverbLevel(m_settings.value("reverbLevel", 15).toInt());
    m_config.setReverbDelay(m_settings.value("reverbDelay", 100).toInt());

    m_config.setBassEnabled(m_settings.value("bassEnabled", true).toBool());
    m_config.setBassLevel(m_settings.value("bassLevel", 50).toInt());
    m_config.setBassCutOff(m_settings.value("bassCutOff", 50).toInt());

    m_config.setSurroundEnabled(m_settings.value("surroundEnabled", true).toBool());
    m_config.setSurroundLevel(m_settings.value("surroundLevel", 20).toInt());
    m_config.setSurroundDelay(m_settings.value("surroundDelay", 15).toInt());

    m_config.setOversamplingEnabled(m_settings.value("oversamplingEnabled", true).toBool());
    m_config.setNoiseReductionEnabled(m_settings.value("noiseReductionEnabled", true).toBool());

    m_settings.endGroup();

    m_config.configureModPlug();
}

void Playback::saveSettings() {
    m_settings.beginGroup("mixer");

    m_settings.setValue("stereo", m_config.stereo());
    m_settings.setValue("sampleSize", m_config.sampleSize());
    m_settings.setValue("frequency", m_config.frequency());

    m_settings.setValue("resamplingMode", m_config.resamplingMode());
    m_settings.setValue("stereoSeparation", m_config.stereoSeparation());
    m_settings.setValue("mixingChannels", m_config.maximumMixingChannels());

    m_settings.setValue("reverbEnabled", m_config.reverbEnabled());
    m_settings.setValue("reverbLevel", m_config.reverbLevel());
    m_settings.setValue("reverbDelay", m_config.reverbDelay());

    m_settings.setValue("bassEnabled", m_config.bassEnabled());
    m_settings.setValue("bassLevel", m_config.bassLevel());
    m_settings.setValue("bassCutOff", m_config.bassCutOff());

    m_settings.setValue("surroundEnabled", m_config.surroundEnabled());
    m_settings.setValue("surroundLevel", m_config.surroundLevel());
    m_settings.setValue("surroundDelay", m_config.surroundDelay());

    m_settings.setValue("oversamplingEnabled", m_config.oversamplingEnabled());
    m_settings.setValue("noiseReductionEnabled", m_config.noiseReductionEnabled());

    m_settings.endGroup();

    m_settings.sync();
}

void Playback::configure() {
    QMutexLocker locker(&m_mutex); // called from user thread
    if(!m_config.isAudioReconfigurationRequired()) {
#ifdef VERBOSE_LOGGING
        qDebug() << "Playback::configure:" << "No audio reconfiguration required";
#endif
        return;
    }
    m_command = ConfigureCommand;
    m_cond.wakeAll();
    while(m_command != NoCommand) {
        m_cond.wait(&m_mutex);
    }
}

void Playback::closePlayback() {
    if(m_mediaPlayer != NULL) {
        m_mediaPlayer->stop();
    }

    // called from playback thread only
    if(m_playback_handle != NULL) {
        ::snd_pcm_close(m_playback_handle);
        m_playback_handle = NULL;
    }
    m_pcmFd = -1;
    m_audioBuffer.resize(0);
}

void Playback::stopAudioDevice() {
    if(m_mediaPlayer != NULL) {
        m_mediaPlayer->stop();
    }
    // called from playback or user thread
    // when called from user thread must be in locked state
    if(m_playback_handle != NULL)
    {
        ::snd_pcm_plugin_flush(m_playback_handle, SND_PCM_CHANNEL_PLAYBACK);
        ::snd_pcm_plugin_playback_drain(m_playback_handle);
        ::snd_pcm_close(m_playback_handle);
        m_playback_handle = NULL;
    }
    m_audioBuffer.resize(0);
    m_pcmFd = -1;
    m_song.rewind();
}

void Playback::changeState(State state) {
    QMutexLocker locker(&m_mutex);
    m_state = state;
    m_cond.wakeAll();
}

void Playback::stopThread() {
    if(isRunning())
    {
        {
            QMutexLocker locker(&m_mutex);
            m_command = ExitCommand;
            m_cond.wakeAll();
        }
        QThread::wait(); // wait till thread is stopped
    }
}

Playback::State Playback::state() {
    return m_state;
}

SongModule* Playback::currentSong() {
    return &m_song;
}

PlaybackConfig* Playback::configuration() {
    return &m_config;
}

bool Playback::submitCommadAndWait(Command command) {
    QMutexLocker locker(&m_mutex);
    m_command = command;
    m_cond.wakeAll();
    while(m_command != NoCommand) {
        m_cond.wait(&m_mutex);
    }
    return true;
}

bool Playback::load(SongExtendedInfo const& info, QString const& fileName) {
    QMutexLocker locker(&m_mutex);
    m_command = LoadCommand;
    m_pendingFileName = fileName;
    m_pendingSong = info;
    m_cond.wakeAll();
    while(m_command != NoCommand) {
        m_cond.wait(&m_mutex);
    }
    return true;
}

bool Playback::unload() {
    return submitCommadAndWait(UnloadCommand);
}

bool Playback::play() {
    Analytics::getInstance()->play();
    return submitCommadAndWait(PlayCommand);
}

bool Playback::stop() {
    Analytics::getInstance()->stop();
    return submitCommadAndWait(StopCommand);
}

bool Playback::pause() {
    Analytics::getInstance()->pause();
    return submitCommadAndWait(PauseCommand);
}

bool Playback::resume() {
    Analytics::getInstance()->resume();
    return submitCommadAndWait(ResumeCommand);
}

bool Playback::rewind() {
    Analytics::getInstance()->rewind();
    return submitCommadAndWait(RewindCommand);
}

void Playback::configureAudio() {
    QString fileName;

    if(m_config.isAudioReconfigurationRequired()) {
#ifdef VERBOSE_LOGGING
        qDebug() << "Audio reconfiguration required";
        qDebug() << "Stopping audio device";
#endif
        stopAudioDevice();
#ifdef VERBOSE_LOGGING
        qDebug() << "Stopped audio device";
        qDebug() << "Initializing audio playback";
#endif
        initPlayback();
#ifdef VERBOSE_LOGGING
        qDebug() << "Audio playback initialized";
#endif
        if(m_song.songLoaded()) {
            fileName = m_song.absoluteFileName();
#ifdef VERBOSE_LOGGING
            qDebug() << "Unloading song module" << fileName;
#endif
            m_song.unload();
#ifdef VERBOSE_LOGGING
            qDebug() << "Song module" << fileName << "unloaded";
#endif
        } else {
#ifdef VERBOSE_LOGGING
            qDebug() << "Song module is not loaded";
#endif
        }
    } else {
#ifdef VERBOSE_LOGGING
        qDebug() << "No audio reconfiguration required";
#endif
    }

    ModPlug_Settings settings;
    memset(&settings, 0, sizeof(settings));

    settings.mFlags = 0;
    if(m_config.oversamplingEnabled()) {
        settings.mFlags |= MODPLUG_ENABLE_OVERSAMPLING;
    }
    if(m_config.noiseReductionEnabled()) {
        settings.mFlags |= MODPLUG_ENABLE_NOISE_REDUCTION;
    }
    if(m_config.reverbEnabled()) {
        settings.mFlags |= MODPLUG_ENABLE_REVERB;
    }
    if(m_config.bassEnabled()) {
        settings.mFlags |= MODPLUG_ENABLE_MEGABASS;
    }
    if(m_config.surroundEnabled()) {
        settings.mFlags |= MODPLUG_ENABLE_SURROUND;
    }

    settings.mChannels = m_config.stereo() ? 2 : 1;
    settings.mBits = m_config.sampleSize();
    settings.mFrequency = m_config.frequency();
    settings.mResamplingMode = m_config.resamplingMode();

    settings.mStereoSeparation = m_config.stereoSeparation();
    settings.mMaxMixChannels = m_config.maximumMixingChannels();

    settings.mReverbDepth = m_config.reverbLevel();
    settings.mReverbDelay = m_config.reverbDelay();

    settings.mBassAmount = m_config.bassLevel();
    settings.mBassRange = m_config.bassCutOff();

    settings.mSurroundDepth = m_config.surroundLevel();
    settings.mSurroundDelay = m_config.surroundDelay();

    settings.mLoopCount = 0;

    ::ModPlug_SetSettings(&settings);

    if(fileName.length() > 0) {
#ifdef VERBOSE_LOGGING
        qDebug() << "Reloading song module" << fileName;
#endif
        m_song.load(m_pendingSong, fileName);
#ifdef VERBOSE_LOGGING
        qDebug() << "Song module" << fileName << "reloaded";
#endif
    }

    saveSettings();
}

void Playback::run() {
#if 0
    moveToThread(this);
#endif
    m_song.moveToThread(this);
#ifdef VERBOSE_LOGGING
    qDebug() << "Entering playback thread";
#endif
    changeState(Idle);

    if(!detectAudioDevice()) {
        changeState(Exit);
        qDebug() << "Failed to detect audio device for playback";
        QThread::exit(1);
        return;
    }
#ifdef VERBOSE_LOGGING
    qDebug() << "Audio device detected successfully";
#endif
    if(!initPlayback()) {
        changeState(Exit);
        qDebug() << "Failed to initialize audio device for playback";
        QThread::exit(2);
        return;
    }
#ifdef VERBOSE_LOGGING
    qDebug() << "Audio device playback initialized successfully";
    qDebug() << "Starting playback loop";
#endif
    m_mutex.lock();
    while(m_state != Exit && m_state != Exiting)
    {
        switch(m_command)
        {
        case NoCommand:
            switch(m_state)
            {
            case Idle:
            case Paused:
            case Loaded:
                m_cond.wait(&m_mutex);
                continue; // process next command
            case Playing:
                if(m_song.isTrackerSong()) {
                    // go playing tracker music
                    break;
                } else {
                    m_cond.wait(&m_mutex);
                    continue; // process next command
                }
            case Exit:
            case Exiting:
                continue; // process next command - exit thread
            }
            break;
        case ExitCommand:
            m_command = NoCommand;
            m_state = Exiting;
            m_cond.wakeAll();
            emit stopped();
            continue; // process next command - exit thread
        case ConfigureCommand:
            m_command = NoCommand;
            m_cond.wakeAll();
            configureAudio();
            continue; // process next command
        case LoadCommand:
            m_command = NoCommand;
            if(m_state != Exiting &&
               m_state != Exit) {
                ::snd_pcm_plugin_flush(m_playback_handle, SND_PCM_CHANNEL_PLAYBACK);
                ::snd_pcm_plugin_playback_drain(m_playback_handle);
                if(m_song.load(m_pendingSong, m_pendingFileName) == true) {
                    m_state = Loaded;
                } else {
                    m_state = Idle;
                }
                m_pendingFileName = "";
                emit stopped();
            }
            m_cond.wakeAll();
            continue; // process next command
        case UnloadCommand:
            m_command = NoCommand;
            if(m_state == Loaded ||
               m_state == Paused ||
               m_state == Playing) {
                if(m_song.isTrackerSong()) {
                    ::snd_pcm_plugin_flush(m_playback_handle, SND_PCM_CHANNEL_PLAYBACK);
                    ::snd_pcm_plugin_playback_drain(m_playback_handle);
                }
                m_song.unload();
                if(m_song.isMp3Song()) {
                    m_mediaPlayer->stop();
                }
                m_state = Idle;
                emit stopped();
            }
            m_cond.wakeAll();
            continue; // process next command
        case RewindCommand:
            m_command = NoCommand;
            if(m_state == Loaded ||
               m_state == Paused ||
               m_state == Playing) {
                m_song.rewind();
                if(m_song.isMp3Song()) {
                    m_mediaPlayer->seekTime(0);
                }
            }
            m_cond.wakeAll();
            continue; // process next command
        case PlayCommand:
            m_command = NoCommand;
            if(m_state == Loaded || m_state == Paused) {
                m_state = Playing;
                if(m_song.isMp3Song()) {
                    bool okToPlay = false;
                    if(m_state == Loaded) {
#ifdef VERBOSE_LOGGING
                        qDebug() << "MediaPlayer: setSourceUrl:" << m_song.absoluteFileName();
#endif
                        bb::multimedia::MediaError::Type mediaError = m_mediaPlayer->setSourceUrl(m_song.absoluteFileName());
                        if(mediaError == bb::multimedia::MediaError::None) {
                            okToPlay = true;
                        } else {
                            qDebug() << "MediaPlayer: setSourceUrl:" << mediaError;
                            okToPlay = false;
                        }
                    } else {
                        okToPlay = true;
                    }
                    if(okToPlay) {
                        bb::multimedia::MediaError::Type mediaError = m_mediaPlayer->play();
                        if(mediaError == bb::multimedia::MediaError::None) {
#ifdef VERBOSE_LOGGING
                            qDebug() << "MediaPlayer: play";
#endif
                            emit playing();
                        } else {
                            qDebug() << "MediaPlayer: play:" << mediaError;
                        }
                    }
                } else {
                    emit playing();
                }
            }
            m_cond.wakeAll();
            continue; // process next command
        case StopCommand:
            m_command = NoCommand;
            if(m_state == Playing || m_state == Paused) {
                m_state = Loaded;
                if(m_song.isTrackerSong()) {
                    ::snd_pcm_plugin_flush(m_playback_handle, SND_PCM_CHANNEL_PLAYBACK);
                    ::snd_pcm_plugin_playback_drain(m_playback_handle);
                }
                m_song.rewind();
                if(m_song.isMp3Song()) {
                    m_mediaPlayer->stop();
                    m_mediaPlayer->seekTime(0);
                }
                emit stopped();
            }
            m_cond.wakeAll();
            continue; // process next command
        case PauseCommand:
            m_command = NoCommand;
            if(m_state == Playing) {
                m_state = Paused;
                if(m_song.isMp3Song()) {
                    m_mediaPlayer->pause();
                }
                emit paused();
            }
            m_cond.wakeAll();
            continue; // process next command
        case ResumeCommand:
            m_command = NoCommand;
            if(m_state == Paused) {
                m_state = Playing;
                if(m_song.isMp3Song()) {
                    m_mediaPlayer->play();
                }
                emit playing();
            }
            m_cond.wakeAll();
            continue; // process next command
        }
        m_mutex.unlock();

        int endOfSongOrError = updateChunk();
        if(endOfSongOrError == 0) // End of song
        {
            changeState(Loaded);
            emit stopped();
            m_song.rewind();
            emit finished();
        }
        else if(endOfSongOrError < 0) // Error in song
        {
            changeState(Idle);
            emit stopped();
            m_song.rewind();
        }

        m_mutex.lock();
    } // while loop

    m_mutex.unlock();
#ifdef VERBOSE_LOGGING
    qDebug() << "Stopping playback...";
#endif
    stopAudioDevice();
#ifdef VERBOSE_LOGGING
    qDebug() << "Exiting playback thread";
#endif
    changeState(Exit);
    QThread::exit(0);
}

bool Playback::detectAudioDevice() {
    int rc;
    qDebug() << "Detecting available audio devices for playback...";
    int ncards = ::snd_cards();
    qDebug() << "Number of audio cards" << ncards;
    char buffer[256];
    for(int i = 0; i < ncards; ++i) {
        rc = ::snd_card_get_name(i, buffer, sizeof(buffer));
        if(rc != -1) {
            qDebug() << "Card" << i << "name:" << buffer;
        }
        rc = ::snd_card_get_longname (i, buffer, sizeof(buffer));
        if(rc != -1) {
            qDebug() << "Card" << i << "long name:" << buffer;
        }
    }
    m_numDevices = 32; /* guess */
    QVector<int> cards(m_numDevices);
    QVector<int> devices(m_numDevices);

    rc = ::snd_pcm_find(SND_PCM_FMT_S16_LE,
                        &m_numDevices,
                        cards.data(),
                        devices.data(),
                        SND_PCM_OPEN_PLAYBACK);
#ifdef VERBOSE_LOGGING
    qDebug() << "Found" << m_numDevices << "audio devices for playback";
#endif
    return rc > 0 && m_numDevices > 0;
}

bool Playback::initPlayback() {
    int err;
    int card;
    int device;

    snd_pcm_format_t            pcm_format;
    snd_pcm_info_t              pcm_info;
    snd_pcm_channel_info_t      channel_info;
    snd_pcm_channel_params_t    channel_params;
    snd_pcm_channel_setup_t     channel_setup;
#ifdef VERBOSE_LOGGING
    qDebug() << "Initializing selected audio device for playback...";
#endif
    if(m_numDevices == 0) {
        qDebug() << "No audio devices available!";
        return false;
    }

    memset(&pcm_format, 0, sizeof(pcm_format));
    switch(m_config.sampleSize())
    {
    case 8:
        pcm_format.format = SND_PCM_SFMT_U8;
        break;
    case 32:
        pcm_format.format = SND_PCM_SFMT_S32_LE;
        break;
    case 16:
    default:
        pcm_format.format = SND_PCM_SFMT_S16_LE;
        break;
    }
    pcm_format.voices = m_config.stereo() ? 2 : 1;
    pcm_format.rate = m_config.frequency();

#if 0
    /*
     * width
The width; one of 8, 16, 24, or 32.
unsigned
0 for signed; 1 for unsigned.
big_endian
0 for little endian; 1 for big endian.*/
    int fmt = ::snd_pcm_build_linear_format(16, 0, 0);
#endif
#ifdef VERBOSE_LOGGING
    qDebug() << "PCM format: voices:" << pcm_format.voices
             << ", rate:" << pcm_format.rate
             << ", format:" << pcm_format.format;
#endif
    if((err = ::snd_pcm_open_preferred(&m_playback_handle,
                                       &card,
                                       &device,
                                       SND_PCM_OPEN_PLAYBACK)) < 0) {
        qDebug() << "Failed to open preferred PCM device:" << ::snd_strerror(err);
        return false;
    } else {
#ifdef VERBOSE_LOGGING
        qDebug() << "Opened preferred PCM device: card" << card << "device" << device;
#endif
    }

    // GET
    if(::snd_pcm_info(m_playback_handle, &pcm_info) != -1) {
#if 0
        qDebug() << "type:" << pcm_info.type;
        qDebug() << "flags:" << pcm_info.flags;
        qDebug() << "id:" << pcm_info.id;
        qDebug() << "name:" << pcm_info.name;
        qDebug() << "playback:" << pcm_info.playback + 1;
        qDebug() << "capture:" << pcm_info.capture + 1;
        qDebug() << "card:" << pcm_info.card;
        qDebug() << "device:" << pcm_info.device;
        qDebug() << "shared_card:" << pcm_info.shared_card;
        qDebug() << "shared_device:" << pcm_info.shared_device;
#endif
    }

    // GET
    m_pcmFd = ::snd_pcm_file_descriptor(m_playback_handle,
                                        SND_PCM_CHANNEL_PLAYBACK);

    /* Get information about a PCM channel's capabilities from a control handle */
    /* https://developer.blackberry.com/native/reference/bb10/audio_libref/topic/libs/snd_pcm_channel_info.html */
    memset(&channel_info, 0, sizeof(channel_info));
    channel_info.channel = SND_PCM_CHANNEL_PLAYBACK;
    if ((err = ::snd_pcm_plugin_info(m_playback_handle, &channel_info)) < 0) {
        qDebug() << "Failed to get PCM plugin info:" << ::snd_strerror(err);
        return false;
    }

#if 0
    qDebug() << "PCM channel info for device" << device << ":";
    qDebug() << "subdevice:" << channel_info.subdevice;
    qDebug() << "subname:" << channel_info.subname;
    qDebug() << "channel:" << channel_info.channel;
    qDebug() << "flags:" << channel_info.flags;
    qDebug() << "formats:" << channel_info.formats;
#define DUMP_FORMAT(x) \
    if(channel_info.formats & x) { \
        qDebug() << " +format:" << #x; \
    } else { \
        qDebug() << " -format:" << #x; \
    }

    DUMP_FORMAT(SND_PCM_FMT_U8)
    DUMP_FORMAT(SND_PCM_FMT_S8)
    DUMP_FORMAT(SND_PCM_FMT_U16_LE)
    DUMP_FORMAT(SND_PCM_FMT_U16_BE)
    DUMP_FORMAT(SND_PCM_FMT_S16_LE)
    DUMP_FORMAT(SND_PCM_FMT_S16_BE)
    DUMP_FORMAT(SND_PCM_FMT_U24_LE)
    DUMP_FORMAT(SND_PCM_FMT_U24_BE)
    DUMP_FORMAT(SND_PCM_FMT_S24_LE)
    DUMP_FORMAT(SND_PCM_FMT_S24_BE)
    DUMP_FORMAT(SND_PCM_FMT_U32_LE)
    DUMP_FORMAT(SND_PCM_FMT_U32_BE)
    DUMP_FORMAT(SND_PCM_FMT_S32_LE)
    DUMP_FORMAT(SND_PCM_FMT_S32_BE)
    DUMP_FORMAT(SND_PCM_FMT_A_LAW)
    DUMP_FORMAT(SND_PCM_FMT_MU_LAW)
    DUMP_FORMAT(SND_PCM_FMT_IEC958_SUBFRAME_LE)
    DUMP_FORMAT(SND_PCM_FMT_IEC958_SUBFRAME_BE)
    DUMP_FORMAT(SND_PCM_FMT_AC3)
    DUMP_FORMAT(SND_PCM_FMT_FLOAT_LE)
    DUMP_FORMAT(SND_PCM_FMT_FLOAT_BE)
    DUMP_FORMAT(SND_PCM_FMT_FLOAT64_LE)
    DUMP_FORMAT(SND_PCM_FMT_FLOAT64_BE)
    DUMP_FORMAT(SND_PCM_FMT_IMA_ADPCM)
    DUMP_FORMAT(SND_PCM_FMT_GSM)
    DUMP_FORMAT(SND_PCM_FMT_MPEG)
    DUMP_FORMAT(SND_PCM_FMT_SPECIAL)

    qDebug() << "rates:" << channel_info.rates;

#define DUMP_RATE(x) \
    if(channel_info.rates & x) { \
        qDebug() << " +rate:" << #x; \
    } else { \
        qDebug() << " -rate:" << #x; \
    }

    DUMP_RATE(SND_PCM_RATE_8000)
    DUMP_RATE(SND_PCM_RATE_11025)
    DUMP_RATE(SND_PCM_RATE_16000)
    DUMP_RATE(SND_PCM_RATE_22050)
    DUMP_RATE(SND_PCM_RATE_32000)
    DUMP_RATE(SND_PCM_RATE_44100)
    DUMP_RATE(SND_PCM_RATE_48000)
    DUMP_RATE(SND_PCM_RATE_88200)
    DUMP_RATE(SND_PCM_RATE_96000)
    DUMP_RATE(SND_PCM_RATE_176400)
    DUMP_RATE(SND_PCM_RATE_192000)
#undef DUMP_RATE

    qDebug() << "min_rate:" << channel_info.min_rate;
    qDebug() << "max_rate:" << channel_info.max_rate;
    qDebug() << "min_voices:" << channel_info.min_voices;
    qDebug() << "max_voices:" << channel_info.max_voices;
    qDebug() << "max_buffer_size:" << channel_info.max_buffer_size;
    qDebug() << "min_fragment_size:" << channel_info.min_fragment_size;
    qDebug() << "max_fragment_size:" << channel_info.max_fragment_size;
    qDebug() << "fragment_align:" << channel_info.fragment_align;
    qDebug() << "fifo_size:" << channel_info.fifo_size;
#endif

    if (channel_info.min_rate > pcm_format.rate ||
        channel_info.max_rate < pcm_format.rate ||
        channel_info.max_voices < pcm_format.voices ||
        !(channel_info.formats & (1 << pcm_format.format))) {
        return false;
    }

    int fragmentSize = channel_info.max_fragment_size;
#if 0
    // SET
    if ((err = snd_pcm_plugin_set_disable(m_playback_handle,
                                          PLUGIN_BUFFER_PARTIAL_BLOCKS)) < 0) {
        fprintf(stderr, "snd_pcm_plugin_set_disable: %s\n", ::snd_strerror(err));
        snd_pcm_close(m_playback_handle);
        m_playback_handle = NULL;
        return err;
    }

    // SET
    if ((err = snd_pcm_plugin_set_disable(m_playback_handle,
                                          PLUGIN_MMAP)) < 0) {
        fprintf(stderr, "snd_pcm_plugin_set_disable: %s\n", ::snd_strerror(err));
        snd_pcm_close(m_playback_handle);
        m_playback_handle = NULL;
        return err;
    }
#endif

    /* Set the configurable parameters for a PCM channel (plugin-aware) */
    memset(&channel_params, 0, sizeof(channel_params));
    channel_params.channel = SND_PCM_CHANNEL_PLAYBACK;
    channel_params.mode = SND_PCM_MODE_BLOCK;

    channel_params.format.interleave = 1;
    channel_params.format.format = pcm_format.format;
    channel_params.format.rate = pcm_format.rate;
    channel_params.format.voices = pcm_format.voices;

    channel_params.start_mode = SND_PCM_START_FULL;
    channel_params.stop_mode = SND_PCM_STOP_STOP;

    channel_params.buf.block.frag_size = fragmentSize;
    channel_params.buf.block.frags_max = 8 - 1;
    channel_params.buf.block.frags_min = 2;

    strcpy(channel_params.sw_mixer_subchn_name, "ModPlayer");

    if ((err = ::snd_pcm_plugin_params(m_playback_handle,
                                       &channel_params)) < 0)
    {
        qDebug() << "Failed to configure PCM plugin:" << ::snd_strerror(err);
        return false;
    }

    if ((err = ::snd_pcm_plugin_prepare(m_playback_handle,
                                        SND_PCM_CHANNEL_PLAYBACK)) < 0)
    {
        qDebug() << "Failed to prepare PCM plugin:" << ::snd_strerror(err);
        return false;
    }

    /* Get the current configuration for the specified PCM channel (plugin aware) */
    memset(&channel_setup, 0, sizeof(channel_setup));
    channel_setup.mode = SND_PCM_MODE_BLOCK;
    channel_setup.channel = SND_PCM_CHANNEL_PLAYBACK;

    if ((err = ::snd_pcm_plugin_setup(m_playback_handle,
                                      &channel_setup)) < 0)
    {
        qDebug() << "Failed to get PCM plugin configuration:" << ::snd_strerror(err);
        return false;
    }

    qDebug() << "Creating" <<  channel_setup.buf.block.frag_size << "bytes audio buffer";
    m_audioBuffer.resize(channel_setup.buf.block.frag_size);

    qDebug() << "Selected audio device was initialized successfully";
    return true;
}

int Playback::updateChunk() {
#ifdef PERFORMANCE_MEASURE
    _Uint64t time_read_start = 0;
    _Uint64t time_read_end = 0;

    _Uint64t time_write_start = 0;
    _Uint64t time_write_end = 0;

    _Uint64t time_update_start = 0;
    _Uint64t time_update_end = 0;

    _Uint64t time_status_end = 0;
    _Uint64t time_status_start = 0;

    _Uint64t time_prepare_start = 0;
    _Uint64t time_prepare_end = 0;

    _Uint64t time_rewrite_start = 0;
    _Uint64t time_rewrite_end = 0;
#endif

    int err;
    int numWritten;
    int bytesGenerated;

#ifdef PERFORMANCE_MEASURE
    time_read_start = get_clock();
#endif

    bytesGenerated = ::ModPlug_Read(m_song,
                                    m_audioBuffer.data(),
                                    m_audioBuffer.size());
    if(bytesGenerated == 0) {
        m_song.update(true);
        return 0;
    }

#ifdef PERFORMANCE_MEASURE
    time_read_end = get_clock();
#endif

#ifdef PERFORMANCE_MEASURE
    time_update_start = get_clock();
#endif
    if(ApplicationUI::instance().isForeground()) {
        m_song.update();
    }
#ifdef PERFORMANCE_MEASURE
    time_update_end = get_clock();
#endif

#ifdef PERFORMANCE_MEASURE
    time_write_start = get_clock();
#endif

    numWritten = ::snd_pcm_plugin_write(m_playback_handle,
                                        m_audioBuffer.data(),
                                        bytesGenerated);
#ifdef PERFORMANCE_MEASURE
    time_write_end = get_clock();
#endif
    if(numWritten < 0)
    {
        qDebug() << "snd_pcm_plugin_write(numWritten < 0): " << errno << "," << strerror(errno);
        if(numWritten == -EINVAL)
        {
            qDebug() << "Failed to write PCM plugin: error=" << errno << "," << strerror(errno);
            m_song.update(true);
            return -1;
        }
    }

    if(numWritten < bytesGenerated) {
#ifdef PERFORMANCE_MEASURE
        time_status_start = get_clock();
#endif
        snd_pcm_channel_status_t status;
        memset(&status, 0, sizeof(status));
        status.channel = SND_PCM_CHANNEL_PLAYBACK;

        if ((err = ::snd_pcm_plugin_status(m_playback_handle, &status)) < 0) {
            qDebug().nospace() << "Failed to get PCM plugin status: error=" << err << "," << ::snd_strerror(err);
            qDebug().space();
            m_song.update(true);
            return -1;
        } else {
#ifdef PERFORMANCE_MEASURE
            time_status_end = get_clock();
#endif
            if(status.status != SND_PCM_STATUS_NOTREADY &&
               status.status != SND_PCM_STATUS_RUNNING)
            {
#ifdef PERFORMANCE_MEASURE
                time_prepare_start = get_clock();
#endif
                if((err = ::snd_pcm_plugin_prepare(m_playback_handle, SND_PCM_CHANNEL_PLAYBACK)) < 0) {
                    qDebug().nospace() << "Failed to prepare PCM plugin: error=" << err << "," << ::snd_strerror(err);
                    qDebug().space();
                    m_song.update(true);
                    return -1;
                } else {
#ifdef PERFORMANCE_MEASURE
                    time_prepare_end = get_clock();

                    time_rewrite_start = get_clock();
#endif
                    numWritten = ::snd_pcm_plugin_write(m_playback_handle,
                                                        m_audioBuffer.data(),
                                                        bytesGenerated);
#ifdef PERFORMANCE_MEASURE
                    time_rewrite_end = get_clock();
#endif
                    if(numWritten != bytesGenerated) {
                        qDebug().nospace() << "Failed to write to PCM plugin: written="
                                           << numWritten
                                           << ", wanted="
                                           << bytesGenerated;
                        qDebug().space();
                        m_song.update(true);
                        return -1;
                    }
                }
            } else {
                qDebug() << "Failed to write to PCM plugin: status=" << status.status;
                m_song.update(true);
                return -1;
            }
        }
    }
#ifdef PERFORMANCE_MEASURE
    qDebug().nospace()
             << "Read="      << (time_read_end - time_read_start) / 1000 << "us"
             << ", Write="   << (time_write_end - time_write_start) / 1000 << "us"
             << ", Update="  << (time_update_end - time_update_start) / 1000 << "us"
             << ", Status="  << (time_status_end - time_status_start) / 1000 << "us"
             << ", Prepare=" << (time_prepare_end - time_prepare_start) / 1000 << "us"
             << ", Rewrite=" << (time_rewrite_end - time_rewrite_start) / 1000 << "us";
    qDebug().space();
#endif
    return bytesGenerated;
}

void Playback::onMediaPlayerBufferStatusChanged(bb::multimedia::BufferStatus::Type type) {
#ifdef VERBOSE_LOGGING
    qDebug() << "Playback::onMediaPlayerBufferStatusChanged" << type;
#endif
}

void Playback::onMediaPlayerError(bb::multimedia::MediaError::Type mediaError, unsigned int position) {
#ifdef VERBOSE_LOGGING
    qDebug().nospace() << "Playback::onMediaPlayerError: error=" << mediaError << ", position=" << position;
    qDebug().space();
#endif
}

void Playback::onMediaPlayerMediaStateChanged(bb::multimedia::MediaState::Type type) {
#ifdef VERBOSE_LOGGING
    qDebug() << "Playback::onMediaPlayerMediaStateChanged" << type;
#endif
}

void Playback::onMediaPlayerMetaDataChanged(const QVariantMap &metaData) {
#ifdef VERBOSE_LOGGING
    qDebug().nospace() << "Playback::onMediaPlayerMetaDataChanged: metadata=" << metaData;
    qDebug().space();
#endif
}

void Playback::onMediaPlayerPlaybackCompleted() {
#ifdef VERBOSE_LOGGING
    qDebug().nospace() << "Playback::onMediaPlayerPlaybackCompleted";
    qDebug().space();
#endif
}

void Playback::onMediaPlayerPositionChanged(unsigned int position) {
#ifdef VERBOSE_LOGGING
    qDebug().nospace() << "Playback::onMediaPlayerPositionChanged: position=" << position;
    qDebug().space();
#endif
}
