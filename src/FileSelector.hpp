#ifndef FILESELECTOR_HPP_
#define FILESELECTOR_HPP_

#include <QObject>
#include <QStringList>

class FileSelector : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(FileSelector)
private:
    QStringList m_filters;
public:
    FileSelector(QStringList const &filters);
    ~FileSelector();
    void start();
    static bool isMp3(QString const& fileName);
    static bool isPlaylist(QString const& fileName);
private:
    void scanDirectory(const char * path,
                       QSet<QString>& foundFiles,
                       QSet<QString>& foundPlaylists);
    bool fileMatches(QString const& fileName);
    bool playlistMatches(QString const& fileName);
    void processPlaylists(QSet<QString> const& playlists,
                          QSet<QString> const& foundFiles);
    void processPlaylist(QString const& playlist,
                         QSet<QString> const& foundFiles);
    static QString createExtensionFilter(QString const& p);
public:
    Q_SIGNAL void searchingDirectory(QString const& fileName);
    Q_SIGNAL void foundFile(QString const& fileName);
    Q_SIGNAL void foundPlaylist(QString const& fileName,
                                QVector<QString> const& songs);
    Q_SIGNAL void done();
private:
    Q_SLOT void selectFiles();
};

#endif
