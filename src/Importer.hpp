#ifndef IMPORTER_HPP
#define IMPORTER_HPP

#include <QObject>
#include <QString>
#include <QStringList>
#include "PlaybackConfig.hpp"
#include "FileSelector.hpp"
#include "MessageBox.hpp"
#include <set>
#include <map>

class QDir;
class Catalog;

class Importer : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Importer)
private:
    FileSelector * m_selector;
    QStringList m_filters;
    Catalog * m_catalog;
    MessageBox m_messageBox;

    std::set<QString> m_knownFileNames;
    std::map<QString, int> m_genreCache;
    std::map<QString, int> m_artistCache;

    int m_numProcessedSongs;
    int m_numProcessedPlaylists;

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
private slots:
    void onFoundFile(QString const& fileName);
    void onFoundPlaylist(QString const& playlistName,
                                QVector<QString> const& songs);
    void onSearchCompleted();
    void onSearchingDirectory(QString const& location);
private:
    void updateLastImportedInfo();
    void removeMissingSongs();
    bool importTrackerSong(QString const& fileName);
    bool importTaggedSong(QString const& fileName);
};

Q_DECLARE_METATYPE(Importer*);

#endif
