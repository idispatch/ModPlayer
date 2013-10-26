#ifndef MP3EXPORT_HPP_
#define MP3EXPORT_HPP_

#include <QObject>
#include <QString>
#include "PlaybackConfig.hpp"

namespace bb {
    namespace system {
#ifdef USE_PROGRESS_TOAST
        class SystemProgressToast;
#else
        class SystemProgressDialog;
#endif
    }
}

class Mp3Export : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Mp3Export)
private:
#ifdef USE_PROGRESS_TOAST
    bb::system::SystemProgressToast * m_progress;
#else
    bb::system::SystemProgressDialog * m_progress;
#endif
public:
    Mp3Export(QObject * parent = 0);
    ~Mp3Export();
    bool convert(PlaybackConfig &config,
                 QString const& inputFileName,
                 QString const& outputFileName);
private:
    void createProgressUI(QString const& fileName);
    void updateProgressUI(int progress);
    void destroyProgressUI();
};

Q_DECLARE_METATYPE(Mp3Export*);

#endif
