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

    Q_INVOKABLE void logError(char const * error, QString const& message);

    Q_INVOKABLE void nowPlaying();
    Q_INVOKABLE void purgeCache();

    Q_INVOKABLE void shareSong(QString const& fileName);
    Q_INVOKABLE void email();
    Q_INVOKABLE void twit();
    Q_INVOKABLE void bbm();
    Q_INVOKABLE void appWorld();
    Q_INVOKABLE void help(int on);
    Q_INVOKABLE void settings(int on);
    Q_INVOKABLE void active(int on);
    Q_INVOKABLE void buy(int on);
    Q_INVOKABLE void purchase(QString const& info);
    Q_INVOKABLE void invoke(QString const& source,
                            QString const& target,
                            QString const& action,
                            QString const& mimeType,
                            QUrl const& uri);

    Q_INVOKABLE void downloadStarted();
    Q_INVOKABLE void downloadFinished();
    Q_INVOKABLE void downloadFailed(int moduleId);

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void rewind();

    Q_INVOKABLE void showPage(QString const& name);
    Q_INVOKABLE void view(int moduleId, QString const& fileName);
    Q_INVOKABLE void search(QString const& query);
    Q_INVOKABLE void play(int moduleId, QString const& fileName);
    Q_INVOKABLE void addFavourite(int moduleId, QString const& fileName);
    Q_INVOKABLE void removeFavourite(int moduleId, QString const& fileName);

    Q_INVOKABLE void resetPlayCounts();
    Q_INVOKABLE void resetMyFavourites();

    Q_INVOKABLE void saveCache(QString const& from, QString const& to);
    Q_INVOKABLE void exportCache(int numFiles, int numCopiedFiles);
    Q_INVOKABLE void exportMp3(QString const& from, QString const& to);
    Q_INVOKABLE void importSongs(bool started);
    Q_INVOKABLE void importedSongCount(int count);
    Q_INVOKABLE void importedPlaylistCount(int count);

    Q_INVOKABLE void createPlaylist(QString const& name);
    Q_INVOKABLE void deleteAllPlaylists();

    Q_INVOKABLE void createAlbum(QString const& artistName, QString const& albumName);

    Q_INVOKABLE void playRadio(QString const& name);
    Q_INVOKABLE void selectRadio(QString const& name);

    using InstanceCounter<Analytics>::getInstanceCount;
    using InstanceCounter<Analytics>::getMaxInstanceCount;
private Q_SLOTS:
    void onPositionUpdated(QGeoPositionInfo const &info);
private:
    Q_DISABLE_COPY(Analytics)
private:
    void setPositionInfo(QGeoPositionInfo const &info);
    void logModuleEvent(const char * eventName,
                        int moduleId,
                        QString const& fileName);

    QtMobilitySubset::QGeoPositionInfoSource * m_source;
    static Analytics * instance;
};

Q_DECLARE_METATYPE(Analytics*);

#endif
