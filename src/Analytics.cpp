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


void Analytics::email() {
    Flurry::Analytics::LogEvent("EmailAuthor", false);
}

void Analytics::twit() {
    Flurry::Analytics::LogEvent("Twit", false);

}

void Analytics::bbm() {
    Flurry::Analytics::LogEvent("BBM", false);
}

void Analytics::failedDownload(int moduleId) {
    Flurry::Map parameters;
    parameters["ModID"] = moduleId;
    Flurry::Analytics::LogEvent("FailedDownload", parameters, false);
}

void Analytics::play(int moduleId) {
    Flurry::Map parameters;
    parameters["ModID"] = moduleId;
    Flurry::Analytics::LogEvent("Play", parameters, false);
}

void Analytics::addFavourite(int moduleId) {
    Flurry::Map parameters;
    parameters["ModID"] = moduleId;
    Flurry::Analytics::LogEvent("AddFavourite", parameters, false);
}

void Analytics::removeFavourite(int moduleId) {
    Flurry::Map parameters;
    parameters["ModID"] = moduleId;
    Flurry::Analytics::LogEvent("RemoveFavourite", parameters, false);
}

void Analytics::resetPlayCounts() {
    Flurry::Analytics::LogEvent("ResetPlayCounts", false);
}
void Analytics::resetMyFavourites() {
    Flurry::Analytics::LogEvent("ResetMyFavourites", false);
}
