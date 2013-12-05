#ifndef IMPORTER_HPP_
#define IMPORTER_HPP_

#include <QObject>
#include <QString>
#include "PlaybackConfig.hpp"

namespace bb {
    namespace system {
        class SystemProgressDialog;
    }
}

class Importer : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Importer)
private:
    bb::system::SystemProgressDialog * m_progress;
public:
    Importer(QObject * parent = 0);
    ~Importer();

    void clean();
private:
    void destroyProgressUI();
    void createProgressUI();
};

Q_DECLARE_METATYPE(Importer*);

#endif
