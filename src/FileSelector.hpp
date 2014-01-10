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
private:
    void scanDirectory(const char * path);
    bool fileMatches(QString const& fileName);
    static QString createExtensionFilter(QString const& p);
public:
    Q_SIGNAL void searchingDirectory(QString const& fileName);
    Q_SIGNAL void foundFile(QString const& fileName);
    Q_SIGNAL void done();
private:
    Q_SLOT void selectFiles();
};

#endif
