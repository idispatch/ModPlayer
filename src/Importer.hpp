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
class Catalog;

class Importer : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Importer)
private:
    Catalog * m_catalog;
    bb::system::SystemProgressDialog * m_progress;
    QStringList m_filters;
    int m_numImportedSongs;
    static const int INDEFINITE = -1;
public:
    Importer(QStringList const& filters,
             Catalog * catalog,
             QObject * parent = 0);
    ~Importer();

    int numImportedSongs() const;

    void clean();
    int import();
private:
    int scanDirectory(QDir const& dir);
    bool importFile(QString const& fileName);

    void createProgressUI();
    void destroyProgressUI();
    void completeProgressUI();
    void updateProgressUI(QString const& body, int progress);
};

Q_DECLARE_METATYPE(Importer*);

#endif
