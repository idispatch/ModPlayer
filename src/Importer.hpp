#ifndef IMPORTER_HPP_
#define IMPORTER_HPP_

#include <QObject>
#include <QString>
#include <QStringList>
#include "PlaybackConfig.hpp"

namespace bb {
    namespace system {
        class SystemProgressDialog;
    }
}

class QDir;

class Importer : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Importer)
private:
    bb::system::SystemProgressDialog * m_progress;
    QStringList m_filters;
    QStringList m_result;
    static const int INDEFINITE = -1;
public:
    Importer(QStringList const& filters,
             QObject * parent = 0);
    ~Importer();

    void clean();
    void import();
private:
    void scanDirectory(QDir const& dir);
    bool importFile(QString const& fileName, int progress);

    void createProgressUI();
    void destroyProgressUI();
    void completeProgressUI();
    void updateProgressUI(QString const& body, int progress);
};

Q_DECLARE_METATYPE(Importer*);

#endif
