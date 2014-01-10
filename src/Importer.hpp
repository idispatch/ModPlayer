#ifndef IMPORTER_HPP_
#define IMPORTER_HPP_

#include <QObject>
#include <QString>
#include <QStringList>
#include "PlaybackConfig.hpp"
#include "MessageBox.hpp"
#include "libmad/mad.h"

class QDir;
class Catalog;

class Importer : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Importer)
private:
    QStringList m_filters;
    Catalog * m_catalog;
    MessageBox m_messageBox;
    int m_numImportedSongs;
public:
    Importer(QStringList const& filters,
             Catalog * catalog,
             QObject * parent = 0);
    ~Importer();
public:
    Q_SLOT void start();
    Q_SIGNAL void searchCompleted();
    int numImportedSongs() const;
private:
    Q_SLOT void onFoundFile(QString const& fileName);
    Q_SLOT void onSearchCompleted();
    Q_SLOT void onSearchingDirectory(QString const& location);
private:
    void clean();

    bool importTrackerSong(QString const& fileName);
    bool importMp3File(QString const& fileName);
    QString getMp3Attribute(void const * tag, const char * attributeName);

    int calculateMp3Duration(char const *path, mad_timer_t *duration, signed int *kbps, unsigned long *kbytes);
    int scanMp3(unsigned char const *ptr, unsigned long len, mad_timer_t *duration);
};

Q_DECLARE_METATYPE(Importer*);

#endif
