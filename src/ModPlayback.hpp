#ifndef MODPLAYBACK_HPP_
#define MODPLAYBACK_HPP_

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QByteArray>
#include <sys/asoundlib.h>

struct _ModPlugFile;
typedef struct _ModPlugFile ModPlugFile;

class ModPlayback : public QThread {
    Q_OBJECT

    enum State {
        Idle,      /* no module loaded and not playing, thread loop is running */
        Loading,   /* module (un)loaded and not playing, thread loop is running */
        Loaded,    /* module loaded and stopped */
        Unloading, /* module loaded and requested to unload */
        Playing,   /* module loaded and playing */
        Paused,    /* module loaded and paused */
        //Seeking,   /* module loaded and seeking */
        //Rewinding, /* module loaded and rewinding */
        Exiting,   /* exit requested */
        Exit       /* no module loaded and exited thread loop */
    };
public:
    ModPlayback(QObject * parent = 0);
    virtual ~ModPlayback();

    void run();
    void stopThread();

    bool load(ModPlugFile * module);
    bool unload();

    bool play();
    bool stop();
    bool pause();
    bool resume();
    bool rewind();
    bool seek(int msec);
Q_SIGNALS:
private:
    Q_DISABLE_COPY(ModPlayback)

    bool detectAudioDevice();
    bool initPlayback();
    int  updateChunk(ModPlugFile * module);
    void stopAudioDevice();
    void closePlayback();
    void waitWhile(State state);
private:
    QMutex m_mutex;
    QWaitCondition m_cond;
    State m_state;

    ModPlugFile * m_module;
    QByteArray m_audioBuffer;
    snd_pcm_t * m_playback_handle;

    bool m_bStereo;
    int m_frequency;
    int m_sampleBitSize;

    int m_numDevices;
    int m_pcmFd;
};

#endif
