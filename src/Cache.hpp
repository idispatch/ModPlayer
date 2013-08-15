#ifndef CACHE_HPP_p
#define CACHE_HPP_

#include <QDebug>
#include <QObject>
#include <QStringList>
#include <QFileInfoList>

class Cache : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString cachePath READ cachePath FINAL)
    Q_PROPERTY(int maxSize READ maxSize WRITE setMaxSize NOTIFY maxSizeChanged FINAL)
    Q_PROPERTY(int maxFiles READ maxFiles WRITE setMaxFiles NOTIFY maxFilesChanged FINAL)
    Q_PROPERTY(int currentSize READ currentSize NOTIFY currentSizeChanged FINAL)
    Q_PROPERTY(int currentFiles READ currentFiles NOTIFY currentFilesChanged FINAL)
    Q_PROPERTY(QStringList files READ files NOTIFY filesChanged FINAL)
    Q_PROPERTY(QStringList fileNameFilters READ fileNameFilters WRITE setFileNameFilters FINAL)

public:
    Cache(int maxSize, int maxFiles, QObject *parent = 0);

    QString cachePath() const;
    QStringList files() const;

    QStringList fileNameFilters() const;
    void setFileNameFilters(QStringList const& value);

    int maxSize() const;
    void setMaxSize(int size);

    int maxFiles() const;
    void setMaxFiles(int size);

    int currentSize() const;
    int currentFiles() const;

    Q_INVOKABLE void purge();
    Q_INVOKABLE void cache(QString const& fileName);
    Q_INVOKABLE bool exists(QString const& fileName);
    Q_INVOKABLE void remove(QString const& fileName);

Q_SIGNALS:
    void maxSizeChanged();
    void maxFilesChanged();
    void currentSizeChanged();
    void currentFilesChanged();
    void filesChanged();
private:
    Q_DISABLE_COPY(Cache)

    QString absoluteFileName(QString const& fileName) const;
    void remove(QFileInfo const& fileInfo);
    void houseKeep();
    void initCache();
    void notifyCacheChanged(int oldFiles, int oldSize);
private:
    QStringList m_fileNameFilters;
    QFileInfoList m_files;
    int m_maxSize;
    int m_maxFiles;
};

Q_DECLARE_METATYPE(Cache*);

QDebug operator << (QDebug dbg, Cache const &c);

#endif
