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
    int m_fragmentSize;
    int m_numDevices;
    int m_pcmFd;
};

#endif
