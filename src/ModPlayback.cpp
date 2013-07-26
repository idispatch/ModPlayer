#include "ModPlayback.hpp"

#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <errno.h>
#include <QDebug>

#include "modplug.h"

ModPlayback::ModPlayback(ModPlugFile * module,
                         QObject * parent)
    : QThread(parent),
      m_module(module),
      m_playback_handle(NULL),
      m_numDevices(0),
      m_pcmFd(-1) {
}

ModPlayback::~ModPlayback() {
    if(m_playback_handle != NULL) {
        snd_pcm_close(m_playback_handle);
        m_playback_handle = NULL;
    }
}

void ModPlayback::run() {
    qDebug() << "Entering thread";
    qDebug() << "Preparing for playback...";

    if(!detectAudioDevice()) {
        qDebug() << "Failed to detect audio device for playback";
        exit(1);
        return;
    }

    qDebug() << "Audio device detected successfully";

    if(!initPlayback()) {
        qDebug() << "Failed to initialize audio device for playback";
        exit(2);
        return;
    }

    qDebug() << "Audio device playback initialized successfully";

    while(1) {
        fd_set wset;
        FD_ZERO(&wset);
        FD_SET(m_pcmFd, &wset);
        int rc = select(1 + m_pcmFd, NULL, &wset, NULL, NULL);
        if(rc == -1) {
            if(errno == EINTR) {
                rc = 0;
                break;
            } else {
                fprintf(stderr, "select: %s\n", strerror(errno));
                rc = errno;
                break;
            }
        } else if FD_ISSET(m_pcmFd, &wset) {
            if(!updateChunk()) {
                break;
            }
        } else {
            fprintf(stderr, "select: unexpected return %d, pcm fd is not ready for write\n", rc);
            break;
        }
    }

    qDebug() << "Stoping playback...";
    stopPlayback();
    qDebug() << "Exiting thread";
    exit(0);
}

bool ModPlayback::detectAudioDevice() {
    qDebug() << "Detecting audio device for playback...";
    int ncards = snd_cards();
    qDebug() << "Number of audio cards" << ncards;
#if 0
    char buffer[256];
    for(int i = 0; i < ncards; ++i) {
        rc = snd_card_get_name(i, buffer, sizeof(buffer));
        if(rc != -1) {
            fprintf(stderr, "Card %d name: %s\n", i, buffer);
        }
        rc = snd_card_get_longname (i, buffer, sizeof(buffer));
        if(rc != -1) {
            fprintf(stderr, "Card %d long name: %s\n", i, buffer);
        }
    }
#endif
    int ret_val;
    int *cards = NULL, *devices  = NULL;
    ret_val = m_numDevices = 32;

    cards = (int*)malloc(sizeof(int) * m_numDevices);
    devices = (int*)malloc(sizeof(int) * m_numDevices);
    memset(cards, 0, sizeof(sizeof(int) * m_numDevices));
    memset(devices, 0, sizeof(sizeof(int) * m_numDevices));
    ret_val = snd_pcm_find(SND_PCM_FMT_S16_LE,
                            &m_numDevices,
                            cards,
                            devices,
                            SND_PCM_OPEN_PLAYBACK);

    qDebug() << "Found" << m_numDevices << "audio devices for playback";

    free(cards);
    free(devices);

    return ret_val > 0 && m_numDevices > 0;
}

bool ModPlayback::initPlayback() {
    int err;
    int card, device;

    snd_pcm_format_t            pcm_format;
    snd_pcm_info_t              pcm_info;
    snd_pcm_channel_info_t      channel_info;
    snd_pcm_channel_params_t    channel_params;
    snd_pcm_channel_setup_t     channel_setup;

    if(m_numDevices == 0) {
        return false;
    }

    memset(&pcm_format, 0, sizeof(pcm_format));
    pcm_format.format = SND_PCM_SFMT_S16_LE;// : SND_PCM_SFMT_U8; // 16vs 8 bits
    pcm_format.voices = 2; // stereo/mono
    pcm_format.rate = 44100; // frequency

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

#if 0
    fprintf(stderr, "PCM format: voices: %d, rate: %d, format: %d\n",
            pcm_format.voices,
            pcm_format.rate,
            pcm_format.format);
#endif

    // GET
    if((err = snd_pcm_open_preferred(&m_playback_handle,
                                     &card,
                                     &device,
                                     SND_PCM_OPEN_PLAYBACK)) < 0) {
        fprintf(stderr, "snd_pcm_open_preferred: %s\n", snd_strerror(err));
        return false;
    } else {
        fprintf(stderr, "snd_pcm_open_preferred: success [card:%d, device:%d]\n", card, device);
    }

    // GET
    if(snd_pcm_info(m_playback_handle, &pcm_info) != -1) {
#if 0
        fprintf(stderr, "type:  %d\n", pcm_info.type);
        fprintf(stderr, "flags: %d\n", pcm_info.flags);
        fprintf(stderr, "id:    %s\n", pcm_info.id);
        fprintf(stderr, "name:  %s\n", pcm_info.name);
        fprintf(stderr, "playback:  %d\n", pcm_info.playback + 1);
        fprintf(stderr, "capture:  %d\n", pcm_info.capture + 1);
        fprintf(stderr, "card:  %d\n", pcm_info.card);
        fprintf(stderr, "device:  %d\n", pcm_info.device);
        fprintf(stderr, "shared_card:  %d\n", pcm_info.shared_card);
        fprintf(stderr, "shared_device:  %d\n", pcm_info.shared_device);
#endif
    }

    // GET
    m_pcmFd = snd_pcm_file_descriptor(m_playback_handle, SND_PCM_CHANNEL_PLAYBACK);

    /* https://developer.blackberry.com/native/reference/bb10/audio_libref/topic/libs/snd_pcm_channel_info.html */
    memset(&channel_info, 0, sizeof(channel_info));
    channel_info.channel = SND_PCM_CHANNEL_PLAYBACK;

    /* Get information about a PCM channel's capabilities from a control handle */
    if ((err = snd_pcm_plugin_info(m_playback_handle, &channel_info)) < 0) {
        fprintf(stderr, "snd_ctl_pcm_plugin_info: %s\n", snd_strerror(err));
        return false;
    }

#if 0
    fprintf(stderr, "PCM channel info for device %d:\n", device);
    fprintf(stderr, "subdevice:  %d\n", channel_info.subdevice);
    fprintf(stderr, "subname:    %s\n", channel_info.subname);
    fprintf(stderr, "channel:    %d\n", channel_info.channel);
    fprintf(stderr, "flags:      %d\n", channel_info.flags);
    fprintf(stderr, "formats:    %d\n", channel_info.formats);
#define DUMP_FORMAT(x) \
    if(channel_info.formats & x) { \
        fprintf(stderr, " +format:      " #x "\n"); \
    } else { \
        fprintf(stderr, " -format:      " #x "\n"); \
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

    fprintf(stderr, "rates:      %d\n", channel_info.rates);

#define DUMP_RATE(x) \
    if(channel_info.rates & x) { \
        fprintf(stderr, " +rate:      " #x "\n"); \
    } else { \
        fprintf(stderr, " -rate:      " #x "\n"); \
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

    fprintf(stderr, "min_rate:   %d\n", channel_info.min_rate);
    fprintf(stderr, "max_rate:   %d\n", channel_info.max_rate);
    fprintf(stderr, "min_voices: %d\n", channel_info.min_voices);
    fprintf(stderr, "max_voices: %d\n", channel_info.max_voices);
    fprintf(stderr, "max_buffer_size: %d\n", channel_info.max_buffer_size);
    fprintf(stderr, "min_fragment_size: %d\n", channel_info.min_fragment_size);
    fprintf(stderr, "max_fragment_size: %d\n", channel_info.max_fragment_size);
    fprintf(stderr, "fragment_align: %d\n", channel_info.fragment_align);
    fprintf(stderr, "fifo_size:  %d\n", channel_info.fifo_size);
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

    if ((err = snd_pcm_plugin_params(m_playback_handle, &channel_params)) < 0) {
        fprintf(stderr, "snd_pcm_plugin_params: %s\n", snd_strerror(err));
        return false;
    }

    if ((err = snd_pcm_plugin_prepare(m_playback_handle, SND_PCM_CHANNEL_PLAYBACK)) < 0) {
        fprintf(stderr, "snd_pcm_plugin_prepare: %s\n", snd_strerror(err));
        return false;
    }

    /* Get the current configuration for the specified PCM channel (plugin aware) */
    memset(&channel_setup, 0, sizeof(channel_setup));
    channel_setup.mode = SND_PCM_MODE_BLOCK;
    channel_setup.channel = SND_PCM_CHANNEL_PLAYBACK;

    if ((err = snd_pcm_plugin_setup(m_playback_handle, &channel_setup)) < 0) {
        fprintf(stderr, "snd_pcm_plugin_setup: %s\n", snd_strerror(err));
        return false;
    }

    m_audioBuffer.resize(channel_setup.buf.block.frag_size);

    return true;
}

void ModPlayback::stopPlayback() {
    if(m_playback_handle != NULL) {
        snd_pcm_playback_drain(m_playback_handle);
        snd_pcm_close(m_playback_handle);
        m_playback_handle = NULL;
    }
    m_audioBuffer.resize(0);
    m_numDevices = 0;
    m_pcmFd = -1;
}

bool ModPlayback::updateChunk() {
    int err;
    int numWritten;
    int bytesGenerated;

    bytesGenerated = ModPlug_Read(m_module,
                                  m_audioBuffer.data(),
                                  m_audioBuffer.size());
    if(bytesGenerated == 0) {
        return false;
    }

    numWritten = snd_pcm_plugin_write(m_playback_handle,
                                      m_audioBuffer.data(),
                                      bytesGenerated);
    if(numWritten < 0) {
        if(numWritten == -EINVAL) {
            fprintf(stderr,"snd_pcm_plugin_write: error=%d (%s)\n", errno, strerror(errno));
            return false;
        }
    }

    if(numWritten < bytesGenerated) {
        snd_pcm_channel_status_t status;
        memset(&status, 0, sizeof(status));
        status.channel = SND_PCM_CHANNEL_PLAYBACK;
        if ((err = snd_pcm_plugin_status(m_playback_handle, &status)) < 0) {
            fprintf(stderr,"snd_pcm_plugin_status: %d (%s)\n", err, snd_strerror(err));
            return false;
        } else {
            if (status.status == SND_PCM_STATUS_READY ||
                status.status == SND_PCM_STATUS_UNDERRUN) {
                if ((err = snd_pcm_plugin_prepare(m_playback_handle, SND_PCM_CHANNEL_PLAYBACK)) < 0) {
                    fprintf(stderr,"snd_pcm_plugin_prepare: %d (%s)\n", err, snd_strerror(err));
                    return false;
                } else {
                    numWritten = snd_pcm_plugin_write(m_playback_handle,
                                                      m_audioBuffer.data(),
                                                      bytesGenerated);
                    if(numWritten != bytesGenerated) {
                        fprintf(stderr,"snd_pcm_plugin_write: %d\n", numWritten);
                        return false;
                    }
                }
            } else {
                fprintf(stderr,"snd_pcm_plugin_write: status %d)\n", status.status);
                return false;
            }
        }
    }
    return true;
}

