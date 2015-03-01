#ifndef IMPORTER_HPP
#define IMPORTER_HPP

#include <QObject>
#include <QString>
#include <QStringList>
#include "PlaybackConfig.hpp"
#include "MessageBox.hpp"
#include <set>

class QDir;
class Catalog;

class Importer : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Importer)
private:
    QStringList m_filters;
    Catalog * m_catalog;
    MessageBox m_messageBox;
    std::set<QString> m_knownFileNames;
    int m_numImportedSongs;
    int m_numImportedPlaylists;
    int m_nextId;
public:
    Importer(QStringList const& filters,
             Catalog * catalog,
             QObject * parent = 0);
    ~Importer();
public:
    static bool lastImportPerformed(QDateTime &date);
    Q_SLOT void start();
    Q_SIGNAL void searchCompleted();
    int numImportedSongs() const;
    int numImportedPlaylists() const;
private:
    Q_SLOT void onFoundFile(QString const& fileName);
    Q_SLOT void onFoundPlaylist(QString const& playlistName,
                                QVector<QString> const& songs);
    Q_SLOT void onSearchCompleted();
    Q_SLOT void onSearchingDirectory(QString const& location);
private:
    void updateLastImportedInfo();
    void removeMissingSongs();
    bool importTrackerSong(QString const& fileName);
    bool importTaggedSong(QString const& fileName);
};

Q_DECLARE_METATYPE(Importer*);

#endif
