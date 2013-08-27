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

    Q_INVOKABLE void nowPlaying();
    Q_INVOKABLE void purgeCache();
    Q_INVOKABLE void email();
    Q_INVOKABLE void twit();
    Q_INVOKABLE void bbm();
    Q_INVOKABLE void help(int on);
    Q_INVOKABLE void settings(int on);

    Q_INVOKABLE void failedDownload(int moduleId);

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void rewind();

    Q_INVOKABLE void view(int moduleId, QString const& fileName);
    Q_INVOKABLE void play(int moduleId, QString const& fileName);
    Q_INVOKABLE void addFavourite(int moduleId, QString const& fileName);
    Q_INVOKABLE void removeFavourite(int moduleId, QString const& fileName);

    Q_INVOKABLE void resetPlayCounts();
    Q_INVOKABLE void resetMyFavourites();

    using InstanceCounter<Analytics>::getInstanceCount;
    using InstanceCounter<Analytics>::getMaxInstanceCount;
Q_SIGNALS:
private Q_SLOTS:
    void onPositionUpdated(QGeoPositionInfo const &info);
private:
    Q_DISABLE_COPY(Analytics)
private:
    void setPositionInfo(QGeoPositionInfo const &info);
    void logModuleEvent(const char * eventName, int moduleId, QString const& fileName);

    QtMobilitySubset::QGeoPositionInfoSource * m_source;
    static Analytics * instance;
};

Q_DECLARE_METATYPE(Analytics*);

#endif
