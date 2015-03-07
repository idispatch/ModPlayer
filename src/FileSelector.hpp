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
private:
    static bool isPlaylist(QString const& fileName);
    void scanDirectory(const char * path,
                       QSet<QString>& foundFiles,
                       QSet<QString>& foundPlaylists);
    bool fileMatches(QString const& fileName) const;
    bool playlistMatches(QString const& fileName) const;
    void processPlaylists(QSet<QString> const& playlists,
                          QSet<QString> const& foundFiles);
    void processPlaylist(QString const& playlist,
                         QSet<QString> const& foundFiles);
Q_SIGNALS:
    void searchingDirectory(QString const& fileName);
    void foundFile(QString const& fileName);
    void foundPlaylist(QString const& fileName,
                       QVector<QString> const& songs);
    void done();
private slots:
    void selectFiles();
};

#endif
