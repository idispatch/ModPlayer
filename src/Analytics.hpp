#ifndef ANALYTICS_HPP_
#define ANALYTICS_HPP_

#include <QObject>
#include <QMetaType>
#include <QtLocationSubset/QGeoPositionInfoSource>
#include <QtLocationSubset/QGeoCoordinate>

#include "InstanceCounter.hpp"

using QtMobilitySubset::QGeoPositionInfo;

class Analytics : public QObject,
                  public InstanceCounter<Analytics> {
    Q_OBJECT
public:
    Analytics(QObject *parent);
    ~Analytics();

    static Analytics * getInstance();

    Q_INVOKABLE void logError(char const * error,
                              QString const& message) const;

    Q_INVOKABLE void nowPlaying() const;
    Q_INVOKABLE void purgeCache() const;
    Q_INVOKABLE void email() const;
    Q_INVOKABLE void twit() const;
    Q_INVOKABLE void bbm() const;
    Q_INVOKABLE void appWorld() const;
    Q_INVOKABLE void help(int on) const;
    Q_INVOKABLE void settings(int on) const;
    Q_INVOKABLE void active(int on) const;
    Q_INVOKABLE void invoke(QString const& source,
                            QString const& target,
                            QString const& action,
                            QString const& mimeType,
                            QUrl const& uri) const;

    Q_INVOKABLE void downloadStarted() const;
    Q_INVOKABLE void downloadFinished() const;
    Q_INVOKABLE void downloadFailed(int moduleId) const;

    Q_INVOKABLE void play() const;
    Q_INVOKABLE void pause() const;
    Q_INVOKABLE void stop() const;
    Q_INVOKABLE void resume() const;
    Q_INVOKABLE void rewind() const;

    Q_INVOKABLE void showPage(QString const& name) const;
    Q_INVOKABLE void view(int moduleId, QString const& fileName) const;
    Q_INVOKABLE void search(QString const& query) const;
    Q_INVOKABLE void play(int moduleId, QString const& fileName) const;
    Q_INVOKABLE void addFavourite(int moduleId, QString const& fileName) const;
    Q_INVOKABLE void removeFavourite(int moduleId, QString const& fileName) const;

    Q_INVOKABLE void resetPlayCounts() const;
    Q_INVOKABLE void resetMyFavourites() const;

    Q_INVOKABLE void saveCache(QString const& from, QString const& to) const;
    Q_INVOKABLE void exportCache(int numFiles, int numCopiedFiles) const;
    Q_INVOKABLE void exportMp3(QString const& from, QString const& to) const;
    Q_INVOKABLE void importSongs(bool started) const;
    Q_INVOKABLE void importedSongCount(int count) const;

    Q_INVOKABLE void createPlaylist(QString const& name) const;
    Q_INVOKABLE void deleteAllPlaylists() const;

    Q_INVOKABLE void createAlbum(QString const& artistName, QString const& albumName) const;

    using InstanceCounter<Analytics>::getInstanceCount;
    using InstanceCounter<Analytics>::getMaxInstanceCount;
Q_SIGNALS:
private Q_SLOTS:
    void onPositionUpdated(QGeoPositionInfo const &info);
private:
    Q_DISABLE_COPY(Analytics)
private:
    void setPositionInfo(QGeoPositionInfo const &info);
    void logModuleEvent(const char * eventName,
                        int moduleId,
                        QString const& fileName) const;

    QtMobilitySubset::QGeoPositionInfoSource * m_source;
    static Analytics * instance;
};

Q_DECLARE_METATYPE(Analytics*);

#endif
