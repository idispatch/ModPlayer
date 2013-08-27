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

void Analytics::nowPlaying() {
    Flurry::Analytics::LogEvent("NowPlaying", false);
}

void Analytics::purgeCache() {
    Flurry::Analytics::LogEvent("PurgeCache", false);
}

void Analytics::email() {
    Flurry::Analytics::LogEvent("EmailAuthor", false);
}

void Analytics::twit() {
    Flurry::Analytics::LogEvent("Twit", false);

}

void Analytics::bbm() {
    Flurry::Analytics::LogEvent("BBM", false);
}

void Analytics::downloadStarted() {
    Flurry::Analytics::LogEvent("Downloading", true);
}

void Analytics::downloadFinished() {
    Flurry::Analytics::EndTimedEvent("Downloading");
}

void Analytics::downloadFailed(int moduleId) {
    Flurry::Map parameters;
    parameters["ModID"] = moduleId;
    Flurry::Analytics::EndTimedEvent("Downloading");
    Flurry::Analytics::LogEvent("DownloadFailed", parameters, false);
}

void Analytics::showPage(QString const& name) {
    Flurry::Map parameters;
    parameters["Page"] = name;
    Flurry::Analytics::LogEvent("ShowPage", parameters, false);
}

void Analytics::view(int moduleId, QString const& fileName) {
    logModuleEvent("View", moduleId, fileName);
}

void Analytics::play(int moduleId, QString const& fileName) {
    logModuleEvent("Play", moduleId, fileName);
}

void Analytics::addFavourite(int moduleId, QString const& fileName) {
    logModuleEvent("AddFavourite", moduleId, fileName);
}

void Analytics::removeFavourite(int moduleId, QString const& fileName) {
    logModuleEvent("RemoveFavourite", moduleId, fileName);
}

void Analytics::logModuleEvent(const char * eventName, int moduleId, QString const& fileName) {
    Flurry::Map parameters;
    parameters["ModID"] = moduleId;
    parameters["FileName"] = fileName;
    Flurry::Analytics::LogEvent(eventName, parameters, false);
}

void Analytics::resetPlayCounts() {
    Flurry::Analytics::LogEvent("ResetPlayCounts", false);
}
void Analytics::resetMyFavourites() {
    Flurry::Analytics::LogEvent("ResetMyFavourites", false);
}

void Analytics::play() {
    Flurry::Analytics::LogEvent("PlayCmd", false);
}

void Analytics::pause() {
    Flurry::Analytics::LogEvent("PauseCmd", false);
}

void Analytics::stop() {
    Flurry::Analytics::LogEvent("StopCmd", false);
}

void Analytics::resume() {
    Flurry::Analytics::LogEvent("ResumeCmd", false);
}

void Analytics::rewind() {
    Flurry::Analytics::LogEvent("RewindCmd", false);
}

void Analytics::help(int on) {
    if(on) {
        Flurry::Analytics::LogEvent("Help", true);
    } else {
        Flurry::Analytics::EndTimedEvent("Help");
    }
}

void Analytics::settings(int on) {
    if(on) {
        Flurry::Analytics::LogEvent("Settings", true);
    } else {
        Flurry::Analytics::EndTimedEvent("Settings");
    }
}

void Analytics::active(int on) {
    Flurry::Map parameters;
    parameters["Time"] = QDateTime::currentDateTime();
    if(on) {
        Flurry::Analytics::LogEvent("Active", parameters, true);
    } else {
        Flurry::Analytics::EndTimedEvent("Active", parameters);
    }
}
