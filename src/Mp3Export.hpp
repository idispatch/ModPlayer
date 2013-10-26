#ifndef MP3EXPORT_HPP_
#define MP3EXPORT_HPP_

#include <QObject>
#include <QString>
#include "PlaybackConfig.hpp"

namespace bb {
    namespace system {
        class SystemProgressToast;
    }
}

class Mp3Export : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Mp3Export)
private:
    bb::system::SystemProgressToast * m_progress;
public:
    Mp3Export(QObject * parent = 0);
    ~Mp3Export();
    bool convert(PlaybackConfig &config,
                 QString const& title,
                 QString const& inputFileName,
                 QString const& outputFileName);
private:
    void createProgressToast(QString const& fileName);
    void updateProgressToast(int progress);
    void destroyProgressToast();
};

Q_DECLARE_METATYPE(Mp3Export*);

#endif
