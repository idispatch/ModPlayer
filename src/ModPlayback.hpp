#ifndef MODPLAYBACK_HPP_
#define MODPLAYBACK_HPP_

#include <QObject>
#include <QThread>
#include <QByteArray>
#include <sys/asoundlib.h>

struct _ModPlugFile;
typedef struct _ModPlugFile ModPlugFile;

class ModPlayback : public QThread {
    Q_OBJECT
public:
    ModPlayback(ModPlugFile * module,
                QObject * parent = 0);
    virtual ~ModPlayback();
    void run();
    void stopThread();
Q_SIGNALS:
private:
    Q_DISABLE_COPY(ModPlayback)

    bool detectAudioDevice();
    bool initPlayback();
    bool updateChunk();
    void stopPlayback();

private:
    ModPlugFile * m_module;
    QByteArray m_audioBuffer;
    snd_pcm_t * m_playback_handle;
    bool m_bStereo;
    int m_frequency;
    int m_sampleBitSize;
    int m_numDevices;
    int m_pcmFd;

    bool m_playRequested;
    bool m_stopRequested;
    bool m_pauseRequested;
    bool m_resumeRequested;
    bool m_rewindRequested;
};

#endif
