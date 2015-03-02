#ifndef CACHE_HPP
#define CACHE_HPP

#include <QDebug>
#include <QObject>
#include <QSettings>
#include <QStringList>
#include <map>
#include <set>

#include "InstanceCounter.hpp"
#include "CacheFileInfo.hpp"

class Cache : public QObject,
              public InstanceCounter<Cache> {
    Q_OBJECT

    Q_PROPERTY(QString cachePath READ cachePath FINAL)
    Q_PROPERTY(int maxSize READ maxSize WRITE setMaxSize NOTIFY maxSizeChanged FINAL)
    Q_PROPERTY(int maxFiles READ maxFiles WRITE setMaxFiles NOTIFY maxFilesChanged FINAL)
    Q_PROPERTY(int currentSize READ currentSize NOTIFY currentSizeChanged FINAL)
    Q_PROPERTY(int currentFiles READ currentFiles NOTIFY currentFilesChanged FINAL)
    Q_PROPERTY(QStringList files READ files NOTIFY filesChanged FINAL)
    Q_PROPERTY(QStringList filters READ filters WRITE setFilters FINAL)

public:
    Cache(QSettings &settings, QObject *parent);
    ~Cache();

    QString cachePath() const;
    QStringList files() const;

    QStringList const& filters() const;
    void setFilters(QStringList const& filters);

    int maxSize() const; // O(1)
    void setMaxSize(int size); // checkOverflow

    int maxFiles() const; // O(1)
    void setMaxFiles(int size); // checkOverflow

    int currentSize() const; // O(N)
    int currentFiles() const; // O(1)

    Q_INVOKABLE void purge();
    Q_INVOKABLE void cache(QString const& fileName);
    Q_INVOKABLE bool fileExists(QString const& fileName);
    Q_INVOKABLE void removeByName(QString const& fileName);
    Q_INVOKABLE void save(QString const& cacheFileName, QString const& newFileName);
    Q_INVOKABLE void exportCache(QString const& directory);

    void initCache();

    using InstanceCounter<Cache>::getInstanceCount;
    using InstanceCounter<Cache>::getMaxInstanceCount;
Q_SIGNALS:
    void progressUpdate(int percent, QString const& fileName);
    void maxSizeChanged();
    void maxFilesChanged();
    void currentSizeChanged();
    void currentFilesChanged();
    void filesChanged();
private:
    Q_DISABLE_COPY(Cache)

    bool fileMatches(QString const& fileName) const;
    QString absoluteFileName(QString const& fileName) const;
    void checkOverflow();
    void notifyCacheChanged(int oldFiles, int oldSize);
private:

    typedef std::map<QString, CacheFileInfo>::const_iterator ConstMapIterator;
    typedef std::set<CacheFileInfo>::const_iterator          ConstSetIterator;

    QSettings                        &m_settings;
    QStringList                       m_filters;
    std::set<CacheFileInfo>           m_sortedFiles;
    std::map<QString, CacheFileInfo>  m_files;
    int                               m_maxSize;
    int                               m_maxFiles;
};

Q_DECLARE_METATYPE(Cache*);

QDebug operator << (QDebug dbg, Cache const &c);

#endif
