#include "ModPlayback.hpp"

#include <string.h>
#include <sys/select.h>
#include <errno.h>
#include <QDebug>
#include <QVector>

#include "modplug.h"

ModPlayback::ModPlayback(QObject * parent)
    : QThread(parent),
      m_mutex(QMutex::NonRecursive),
      m_state(Exit),
      m_command(NoCommand),
      m_song(NULL),
      m_numDevices(0),
      m_pcmFd(-1),
      m_playback_handle(NULL) {
    m_config.configureModPlug();
}

ModPlayback::~ModPlayback() {
    closePlayback(); // called from playback thread only
}

void ModPlayback::configure() {
    QMutexLocker locker(&m_mutex); // called from user thread
    if(!m_config.isAudioReconfigurationRequired()) {
        qDebug() << "No audio reconfiguring required";
        return;
    }
    qDebug() << "Reconfiguring playback";
    m_command = ConfigureCommand;
    m_cond.wakeAll();
    while(m_command != NoCommand) {
        m_cond.wait(&m_mutex);
    }
}

void ModPlayback::closePlayback() {
    // called from playback thread only
    if(m_playback_handle != NULL) {
        snd_pcm_close(m_playback_handle);
        m_playback_handle = NULL;
    }
    m_pcmFd = -1;
    m_audioBuffer.resize(0);
}

void ModPlayback::stopAudioDevice() {
    // called from playback or user thread
    // when called from user thread must be in locked state
    if(m_playback_handle != NULL)
    {
        snd_pcm_playback_drain(m_playback_handle);
        snd_pcm_close(m_playback_handle);
        m_playback_handle = NULL;
    }
    m_audioBuffer.resize(0);
    m_pcmFd = -1;
    m_song.rewind();
}

void ModPlayback::changeState(State state) {
    QMutexLocker locker(&m_mutex);
    m_state = state;
    m_cond.wakeAll();
}

void ModPlayback::stopThread() {
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

ModPlayback::State ModPlayback::state() {
    return m_state;
}

SongModule* ModPlayback::currentSong() {
    return &m_song;
}

PlaybackConfig* ModPlayback::configuration() {
    return &m_config;
}

bool ModPlayback::load(SongInfo const& info, QString const& fileName) {
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

bool ModPlayback::unload() {
    QMutexLocker locker(&m_mutex);
    m_command = UnloadCommand;
    m_cond.wakeAll();
    while(m_command != NoCommand) {
        m_cond.wait(&m_mutex);
    }
    return true;
}

bool ModPlayback::play() {
    QMutexLocker locker(&m_mutex);
    m_command = PlayCommand;
    m_cond.wakeAll();
    while(m_command != NoCommand) {
        m_cond.wait(&m_mutex);
    }
    return true;
}

bool ModPlayback::stop() {
    QMutexLocker locker(&m_mutex);
    m_command = StopCommand;
    m_cond.wakeAll();
    while(m_command != NoCommand) {
        m_cond.wait(&m_mutex);
    }
    return true;
}

bool ModPlayback::pause() {
    QMutexLocker locker(&m_mutex);
    m_command = PauseCommand;
    m_cond.wakeAll();
    while(m_command != NoCommand) {
        m_cond.wait(&m_mutex);
    }
    return true;
}

bool ModPlayback::resume() {
    QMutexLocker locker(&m_mutex);
    m_command = ResumeCommand;
    m_cond.wakeAll();
    while(m_command != NoCommand) {
        m_cond.wait(&m_mutex);
    }
    return true;
}

bool ModPlayback::rewind() {
    QMutexLocker locker(&m_mutex);
    m_command = RewindCommand;
    m_cond.wakeAll();
    while(m_command != NoCommand) {
        m_cond.wait(&m_mutex);
    }
    return true;
}

void ModPlayback::configure_audio() {
    QString fileName;

    if(m_config.isAudioReconfigurationRequired()) {
        qDebug() << "Audio reconfiguration required";
        qDebug() << "Stopping audio device";
        stopAudioDevice();
        qDebug() << "Stopped audio device";
        qDebug() << "Initializing audio playback";
        initPlayback();
        qDebug() << "Audio playback initialized";
        if(m_song.songLoaded()) {
            qDebug() << "Unloading song module";
            fileName = m_song.fileName();
            m_song.unload();
            qDebug() << "Song module unloaded";
        }
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

    ModPlug_SetSettings(&settings);

    if(fileName.length() > 0) {
        qDebug() << "Reloading song module";
        m_song.load(m_pendingSong, fileName);
        qDebug() << "Song module reloaded";
    }
}

void ModPlayback::run() {
    qDebug() << "Entering playback thread";
    changeState(Idle);

    if(!detectAudioDevice()) {
        changeState(Exit);
        qDebug() << "Failed to detect audio device for playback";
        QThread::exit(1);
        return;
    }

    qDebug() << "Audio device detected successfully";

    if(!initPlayback()) {
        changeState(Exit);
        qDebug() << "Failed to initialize audio device for playback";
        QThread::exit(2);
        return;
    }

    qDebug() << "Audio device playback initialized successfully";
    qDebug() << "Starting playback loop";

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
                continue;
            case Playing: // go playing
                break;
            case Exit:
            case Exiting:
                continue;
            }
            break;
        case ExitCommand:
            m_command = NoCommand;
            m_state = Exiting;
            m_cond.wakeAll();
            emit stopped();
            continue;
        case ConfigureCommand:
            m_command = NoCommand;
            m_cond.wakeAll();
            m_mutex.unlock();
            configure_audio();
            m_mutex.lock();
            continue;
        case LoadCommand:
            m_command = NoCommand;
            if(m_state != Exiting &&
               m_state != Exit) {
                if(m_song.load(m_pendingSong, m_pendingFileName)) {
                    m_state = Loaded;
                } else {
                    m_state = Idle;
                }
                m_pendingFileName = "";
                emit stopped();
            }
            m_cond.wakeAll();
            continue;
        case UnloadCommand:
            m_command = NoCommand;
            if(m_state == Loaded ||
               m_state == Paused ||
               m_state == Playing) {
                m_song.unload();
                m_state = Idle;
                emit stopped();
            }
            m_cond.wakeAll();
            continue;
        case RewindCommand:
            m_command = NoCommand;
            if(m_state == Loaded ||
               m_state == Paused ||
               m_state == Playing) {
                m_song.rewind();
            }
            m_cond.wakeAll();
            continue;
        case PlayCommand:
            m_command = NoCommand;
            if(m_state == Loaded || m_state == Paused) {
                m_state = Playing;
                emit playing();
            }
            m_cond.wakeAll();
            continue;
        case StopCommand:
            m_command = NoCommand;
            if(m_state == Playing || m_state == Paused) {
                m_state = Loaded;
                emit stopped();
            }
            m_cond.wakeAll();
            continue;
        case PauseCommand:
            m_command = NoCommand;
            if(m_state == Playing) {
                m_state = Paused;
                emit paused();
            }
            m_cond.wakeAll();
            continue;
        case ResumeCommand:
            m_command = NoCommand;
            if(m_state == Paused) {
                m_state = Playing;
                emit playing();
            }
            m_cond.wakeAll();
            continue;
        }
        m_mutex.unlock();

        fd_set fdSet;
        FD_ZERO(&fdSet);
        FD_SET(m_pcmFd, &fdSet);

        timeval tv = {3, 0};
        int rc = select(1 + m_pcmFd, NULL, &fdSet, NULL, &tv);
        switch(rc)
        {
        case 0: // Timeout
            break;
        case -1: // Error
            if(errno != EINTR && errno != EAGAIN)
            {
                changeState(Exit);
                emit stopped();
            }
            break;
        default:
            if FD_ISSET(m_pcmFd, &fdSet)
            {
                int endOfSongOrError = updateChunk();
                if(endOfSongOrError == 0) // End of song
                {
                    changeState(Loaded);
                    emit stopped();
                }
                else if(endOfSongOrError < 0) // Error in song
                {
                    changeState(Idle);
                    emit stopped();
                }
            }
            break;
        } // switch

        m_mutex.lock();
    } // while loop

    m_mutex.unlock();
    qDebug() << "Stopping playback...";
    stopAudioDevice();
    qDebug() << "Exiting playback thread";
    changeState(Exit);
    QThread::exit(0);
}

bool ModPlayback::detectAudioDevice() {
    int rc;
    qDebug() << "Detecting available audio devices for playback...";
    int ncards = snd_cards();
    qDebug() << "Number of audio cards" << ncards;
    char buffer[256];
    for(int i = 0; i < ncards; ++i) {
        rc = snd_card_get_name(i, buffer, sizeof(buffer));
        if(rc != -1) {
            qDebug() << "Card" << i << "name:" << buffer;
        }
        rc = snd_card_get_longname (i, buffer, sizeof(buffer));
        if(rc != -1) {
            qDebug() << "Card" << i << "long name:" << buffer;
        }
    }
    m_numDevices = 32; /* guess */
    QVector<int> cards(m_numDevices);
    QVector<int> devices(m_numDevices);

    rc = snd_pcm_find(SND_PCM_FMT_S16_LE,
                      &m_numDevices,
                      cards.data(),
                      devices.data(),
                      SND_PCM_OPEN_PLAYBACK);

    qDebug() << "Found" << m_numDevices << "audio devices for playback";

    return rc > 0 && m_numDevices > 0;
}

bool ModPlayback::initPlayback() {
    int err;
    int card;
    int device;

    snd_pcm_format_t            pcm_format;
    snd_pcm_info_t              pcm_info;
    snd_pcm_channel_info_t      channel_info;
    snd_pcm_channel_params_t    channel_params;
    snd_pcm_channel_setup_t     channel_setup;

    qDebug() << "Initializing selected audio device for playback...";

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
    int fmt = snd_pcm_build_linear_format(16, 0, 0);
#endif

    qDebug() << "PCM format: voices:" << pcm_format.voices
             << ", rate:" << pcm_format.rate
             << ", format:" << pcm_format.format;

    if((err = snd_pcm_open_preferred(&m_playback_handle,
                                     &card,
                                     &device,
                                     SND_PCM_OPEN_PLAYBACK)) < 0) {
        qDebug() << "Failed to open preferred PCM device:" << snd_strerror(err);
        return false;
    } else {
        qDebug() << "Opened preferred PCM device: card" << card << "device" << device;
    }

    // GET
    if(snd_pcm_info(m_playback_handle, &pcm_info) != -1) {
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
    m_pcmFd = snd_pcm_file_descriptor(m_playback_handle,
                                      SND_PCM_CHANNEL_PLAYBACK);

    /* Get information about a PCM channel's capabilities from a control handle */
    /* https://developer.blackberry.com/native/reference/bb10/audio_libref/topic/libs/snd_pcm_channel_info.html */
    memset(&channel_info, 0, sizeof(channel_info));
    channel_info.channel = SND_PCM_CHANNEL_PLAYBACK;
    if ((err = snd_pcm_plugin_info(m_playback_handle, &channel_info)) < 0) {
        qDebug() << "Failed to get PCM plugin info:" << snd_strerror(err);
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
        fprintf(stderr, "snd_pcm_plugin_set_disable: %s\n", snd_strerror(err));
        snd_pcm_close(m_playback_handle);
        m_playback_handle = NULL;
        return err;
    }

    // SET
    if ((err = snd_pcm_plugin_set_disable(m_playback_handle,
                                          PLUGIN_MMAP)) < 0) {
        fprintf(stderr, "snd_pcm_plugin_set_disable: %s\n", snd_strerror(err));
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

    if ((err = snd_pcm_plugin_params(m_playback_handle,
                                     &channel_params)) < 0)
    {
        qDebug() << "Failed to configure PCM plugin:" << snd_strerror(err);
        return false;
    }

    if ((err = snd_pcm_plugin_prepare(m_playback_handle,
                                      SND_PCM_CHANNEL_PLAYBACK)) < 0)
    {
        qDebug() << "Failed to prepare PCM plugin:" << snd_strerror(err);
        return false;
    }

    /* Get the current configuration for the specified PCM channel (plugin aware) */
    memset(&channel_setup, 0, sizeof(channel_setup));
    channel_setup.mode = SND_PCM_MODE_BLOCK;
    channel_setup.channel = SND_PCM_CHANNEL_PLAYBACK;

    if ((err = snd_pcm_plugin_setup(m_playback_handle,
                                    &channel_setup)) < 0)
    {
        qDebug() << "Failed to get PCM plugin configuration:" << snd_strerror(err);
        return false;
    }

    qDebug() << "Creating" <<  channel_setup.buf.block.frag_size << "bytes audio buffer";
    m_audioBuffer.resize(channel_setup.buf.block.frag_size);

    qDebug() << "Selected audio device was initialized successfully";
    return true;
}

int ModPlayback::updateChunk() {
    int err;
    int numWritten;
    int bytesGenerated;

    bytesGenerated = ModPlug_Read(m_song,
                                  m_audioBuffer.data(),
                                  m_audioBuffer.size());
    if(bytesGenerated == 0) {
        m_song.update(true);
        return 0;
    }

    m_song.update();

    numWritten = snd_pcm_plugin_write(m_playback_handle,
                                      m_audioBuffer.data(),
                                      bytesGenerated);
    if(numWritten < 0)
    {
        if(numWritten == -EINVAL)
        {
            qDebug() << "Failed to write PCM plugin: error=" << errno << "," << strerror(errno);
            m_song.update(true);
            return -1;
        }
    }

    if(numWritten < bytesGenerated)
    {
        snd_pcm_channel_status_t status;
        memset(&status, 0, sizeof(status));
        status.channel = SND_PCM_CHANNEL_PLAYBACK;

        if ((err = snd_pcm_plugin_status(m_playback_handle, &status)) < 0)
        {
            qDebug() << "Failed to get PCM plugin status: error="
                     << err
                     << ","
                     << snd_strerror(err);
            m_song.update(true);
            return -1;
        }
        else
        {
            if (status.status == SND_PCM_STATUS_READY ||
                status.status == SND_PCM_STATUS_UNDERRUN)
            {
                if((err = snd_pcm_plugin_prepare(m_playback_handle, SND_PCM_CHANNEL_PLAYBACK)) < 0)
                {
                    qDebug() << "Failed to prepare PCM plugin: error="
                             << err
                             << ","
                             << snd_strerror(err);
                    m_song.update(true);
                    return -1;
                }
                else
                {
                    numWritten = snd_pcm_plugin_write(m_playback_handle,
                                                      m_audioBuffer.data(),
                                                      bytesGenerated);
                    if(numWritten != bytesGenerated)
                    {
                        qDebug() << "Failed to write to PCM plugin: written="
                                 << numWritten
                                 << ", wanted="
                                 << bytesGenerated;
                        m_song.update(true);
                        return -1;
                    }
                }
            }
            else
            {
                qDebug() << "Failed to write to PCM plugin: status=" << status.status;
                m_song.update(true);
                return -1;
            }
        }
    }
    return bytesGenerated;
}
