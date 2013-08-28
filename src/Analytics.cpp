#include "Analytics.hpp"
#include "Flurry/Flurry.h"

#include <QDebug>
#include <bb/ApplicationInfo>

using namespace bb;
using namespace QtMobilitySubset;

template<>
int InstanceCounter<Analytics>::s_count;
template<>
int InstanceCounter<Analytics>::s_maxCount;

Analytics * Analytics::instance = 0;

Analytics * Analytics::getInstance() {
    return instance;
}

Analytics::Analytics(QObject *parent)
    : QObject(parent),
      m_source(QGeoPositionInfoSource::createDefaultSource(this)) {
    instance = this;

    ApplicationInfo info;

    qDebug() << "Analytics: agent version=" << Flurry::Analytics::GetFlurryAgentVersion();
    qDebug() << "Analytics: app version" << info.version();

    Flurry::Analytics::SetAppVersion(info.version());
    Flurry::Analytics::SetShowErrorInLogEnabled(true);
    Flurry::Analytics::SetDebugLogEnabled(true);
    Flurry::Analytics::SetSessionContinueSeconds(10);
    Flurry::Analytics::SetSecureTransportEnabled(true);

    Flurry::Analytics::StartSession("RYHJ7FPDTQW22Z7YYTW9");

    Flurry::Analytics::SetSessionReportsOnCloseEnabled(true);
    Flurry::Analytics::SetSessionReportsOnPauseEnabled(true);
    Flurry::Analytics::SetEventLoggingEnabled(true);

    setPositionInfo(m_source->lastKnownPosition());

    //Flurry::Analytics::SetUserID("");
    //Flurry::Analytics::SetAge(0);
    //Flurry::Analytics::SetGender("");

    bool positionUpdatedConnected = QObject::connect(m_source, SIGNAL(positionUpdated(const QGeoPositionInfo &)),
                                                     this,     SLOT(onPositionUpdated(const QGeoPositionInfo &)));

    if (positionUpdatedConnected) {
        m_source->requestUpdate();
    } else {
        qDebug() << "Analytics: positionUpdated connection failed";
        Flurry::Analytics::LogError("positionUpdated connection failed");
    }
}

Analytics::~Analytics() {
    instance = 0;
    //qDebug() << "Analytics::~Analytics()";
}

void Analytics::setPositionInfo(QGeoPositionInfo const &info) {
    if (!info.isValid()) {
        qDebug() << "Analytics::setLocation: invalid position info";
    }
    QGeoCoordinate coordinate = info.coordinate();
    Flurry::Analytics::SetLocation(coordinate.latitude(),
                                   coordinate.longitude(),
                                   info.attribute(QGeoPositionInfo::HorizontalAccuracy),
                                   info.attribute(QGeoPositionInfo::VerticalAccuracy));
}

void Analytics::onPositionUpdated(const QGeoPositionInfo &info) {
    if (!info.isValid()) {
        Flurry::Analytics::LogError("Analytics::onPositionUpdated returned invalid location fix");
        return;
    }
    setPositionInfo(info);
}

void Analytics::nowPlaying() const {
    Flurry::Analytics::LogEvent("NowPlaying", false);
}

void Analytics::purgeCache() const {
    Flurry::Analytics::LogEvent("PurgeCache", false);
}

void Analytics::email() const {
    Flurry::Analytics::LogEvent("EmailAuthor", false);
}

void Analytics::twit() const {
    Flurry::Analytics::LogEvent("Twit", false);
}

void Analytics::bbm() const {
    Flurry::Analytics::LogEvent("BBM", false);
}

void Analytics::appWorld() const {
    Flurry::Analytics::LogEvent("AppWorld", false);
}

void Analytics::downloadStarted() const {
    Flurry::Analytics::LogEvent("Downloading", true);
}

void Analytics::downloadFinished() const {
    Flurry::Analytics::EndTimedEvent("Downloading");
}

void Analytics::downloadFailed(int moduleId) const {
    Flurry::Map parameters;
    parameters["ModID"] = moduleId;
    Flurry::Analytics::EndTimedEvent("Downloading");
    Flurry::Analytics::LogEvent("DownloadFailed", parameters, false);
}

void Analytics::showPage(QString const& name) const {
    Flurry::Map parameters;
    parameters["Page"] = name;
    Flurry::Analytics::LogEvent("ShowPage", parameters, false);
}

void Analytics::view(int moduleId, QString const& fileName) const {
    logModuleEvent("View", moduleId, fileName);
}

void Analytics::play(int moduleId, QString const& fileName) const {
    logModuleEvent("Play", moduleId, fileName);
}

void Analytics::addFavourite(int moduleId, QString const& fileName) const {
    logModuleEvent("AddFavourite", moduleId, fileName);
}

void Analytics::removeFavourite(int moduleId, QString const& fileName) const {
    logModuleEvent("RemoveFavourite", moduleId, fileName);
}

void Analytics::logModuleEvent(const char * eventName,
                               int moduleId,
                               QString const& fileName) const {
    Flurry::Map parameters;
    parameters["ModID"] = moduleId;
    parameters["FileName"] = fileName;
    Flurry::Analytics::LogEvent(eventName, parameters, false);
}

void Analytics::resetPlayCounts() const {
    Flurry::Analytics::LogEvent("ResetPlayCounts", false);
}
void Analytics::resetMyFavourites() const {
    Flurry::Analytics::LogEvent("ResetMyFavourites", false);
}

void Analytics::play() const {
    Flurry::Analytics::LogEvent("PlayCmd", false);
}

void Analytics::pause() const {
    Flurry::Analytics::LogEvent("PauseCmd", false);
}

void Analytics::stop() const {
    Flurry::Analytics::LogEvent("StopCmd", false);
}

void Analytics::resume() const {
    Flurry::Analytics::LogEvent("ResumeCmd", false);
}

void Analytics::rewind() const {
    Flurry::Analytics::LogEvent("RewindCmd", false);
}

void Analytics::help(int on) const {
    if(on) {
        Flurry::Analytics::LogEvent("Help", true);
    } else {
        Flurry::Analytics::EndTimedEvent("Help");
    }
}

void Analytics::settings(int on) const {
    if(on) {
        Flurry::Analytics::LogEvent("Settings", true);
    } else {
        Flurry::Analytics::EndTimedEvent("Settings");
    }
}

void Analytics::active(int on) const {
    Flurry::Map parameters;
    parameters["Time"] = QDateTime::currentDateTime();
    if(on) {
        Flurry::Analytics::LogEvent("Active", parameters, true);
    } else {
        Flurry::Analytics::EndTimedEvent("Active", parameters);
    }
}
