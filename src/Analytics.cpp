#include "Analytics.hpp"
#ifndef __X86__
#include "Flurry/Flurry.h"
#endif
#include <QDebug>
#include <bb/ApplicationInfo>
#include <bb/device/HardwareInfo>

using namespace bb;
using namespace bb::device;
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
#ifndef __X86__
    ApplicationInfo info;

    qDebug() << "Analytics: agent version=" << Flurry::Analytics::GetFlurryAgentVersion();
    qDebug() << "Analytics: app version" << info.version();

    Flurry::Analytics::SetAppVersion(info.version());
    Flurry::Analytics::SetShowErrorInLogEnabled(true);
    Flurry::Analytics::SetDebugLogEnabled(true);
    Flurry::Analytics::SetSessionContinueSeconds(300);
    Flurry::Analytics::SetSecureTransportEnabled(true);

    if(info.title() == "ModPlayer") {
        Flurry::Analytics::StartSession("RYHJ7FPDTQW22Z7YYTW9");
    } else {
        Flurry::Analytics::StartSession("HFMVZJYPDC6JSBHVNK2Q");
    }

    Flurry::Analytics::SetSessionReportsOnCloseEnabled(true);
    Flurry::Analytics::SetSessionReportsOnPauseEnabled(true);
    Flurry::Analytics::SetEventLoggingEnabled(true);

    setPositionInfo(m_source->lastKnownPosition());

    HardwareInfo hwInfo;
    Flurry::Analytics::SetUserID(hwInfo.pin());
    //Flurry::Analytics::SetAge(0);
    //Flurry::Analytics::SetGender("");

    Flurry::Map parameters;
    parameters["pin"] = hwInfo.pin();
    parameters["imei"] = hwInfo.imei();
    parameters["deviceName"] = hwInfo.deviceName();
    parameters["hardwareId"] = hwInfo.hardwareId();
    parameters["meid"] = hwInfo.meid();
    parameters["serial"] = hwInfo.serialNumber();
    Flurry::Analytics::LogEvent("DeviceInfo", parameters, false);

    bool positionUpdatedConnected = QObject::connect(m_source, SIGNAL(positionUpdated(const QGeoPositionInfo &)),
                                                     this,     SLOT(onPositionUpdated(const QGeoPositionInfo &)));

    if (positionUpdatedConnected) {
        m_source->requestUpdate();
    } else {
        qDebug() << "Analytics: positionUpdated connection failed";
        Flurry::Analytics::LogError("positionUpdated connection failed");
    }
#endif
}

Analytics::~Analytics() {
    instance = 0;
    //qDebug() << "Analytics::~Analytics()";
}

void Analytics::setPositionInfo(QGeoPositionInfo const &info) {
    if (!info.isValid()) {
        qDebug() << "Analytics::setLocation: invalid position info";
    }
#ifndef __X86__
    QGeoCoordinate coordinate = info.coordinate();
    Flurry::Analytics::SetLocation(coordinate.latitude(),
                                   coordinate.longitude(),
                                   info.attribute(QGeoPositionInfo::HorizontalAccuracy),
                                   info.attribute(QGeoPositionInfo::VerticalAccuracy));
#endif
}

void Analytics::onPositionUpdated(const QGeoPositionInfo &info) {
#ifndef __X86__
    if (!info.isValid()) {
        Flurry::Analytics::LogError("Analytics::onPositionUpdated returned invalid location fix");
        return;
    }
    setPositionInfo(info);
#endif
}

void Analytics::nowPlaying() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("NowPlaying", false);
#endif
}

void Analytics::purgeCache() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("PurgeCache", false);
#endif
}

void Analytics::email() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("EmailAuthor", false);
#endif
}

void Analytics::twit() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("Twit", false);
#endif
}

void Analytics::bbm() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("BBM", false);
#endif
}

void Analytics::appWorld() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("AppWorld", false);
#endif
}

void Analytics::downloadStarted() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("Downloading", true);
#endif
}

void Analytics::downloadFinished() {
#ifndef __X86__
    Flurry::Analytics::EndTimedEvent("Downloading");
#endif
}

void Analytics::downloadFailed(int moduleId) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["ModID"] = moduleId;
    Flurry::Analytics::EndTimedEvent("Downloading");
    Flurry::Analytics::LogEvent("DownloadFailed", parameters, false);
    logError("DownloadFailed", QString("Module %1").arg(moduleId));
#endif
}

void Analytics::logError(char const * error,
						 QString const& message) {
#ifndef __X86__
    Flurry::Analytics::LogError(error, message);
#endif
}

void Analytics::showPage(QString const& name) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["Page"] = name;
    Flurry::Analytics::LogEvent("ShowPage", parameters, false);
#endif
}

void Analytics::playRadio(QString const& name) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["Channel"] = name;
    Flurry::Analytics::LogEvent("PlayRadio", parameters, false);
#endif
}

void Analytics::selectRadio(QString const& name) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["Channel"] = name;
    Flurry::Analytics::LogEvent("SelectRadio", parameters, false);
#endif
}

void Analytics::view(int moduleId, QString const& fileName) {
    logModuleEvent("View", moduleId, fileName);
}

void Analytics::search(QString const& query) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["Query"] = query;
    Flurry::Analytics::LogEvent("Search", parameters, false);
#endif
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

void Analytics::logModuleEvent(const char * eventName,
                               int moduleId,
                               QString const& fileName) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["ModID"] = moduleId;
    parameters["FileName"] = fileName;
    Flurry::Analytics::LogEvent(eventName, parameters, false);
#endif
}

void Analytics::resetPlayCounts() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("ResetPlayCounts", false);
#endif
}

void Analytics::resetMyFavourites() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("ResetMyFavourites", false);
#endif
}

void Analytics::saveCache(QString const& from, QString const& to) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["fromFile"] = from;
    parameters["toFile"] = to;
    Flurry::Analytics::LogEvent("SaveCache", parameters, false);
#endif
}

void Analytics::exportCache(int numFiles, int numCopiedFiles) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["cacheFiles"] = numFiles;
    parameters["copiedFiles"] = numCopiedFiles;
    Flurry::Analytics::LogEvent("ExportCache", parameters, false);
#endif
}

void Analytics::exportMp3(QString const& from, QString const& to) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["fromFile"] = from;
    parameters["toFile"] = to;
    Flurry::Analytics::LogEvent("ExportMp3", parameters, false);
#endif
}

void Analytics::importSongs(bool started) {
#ifndef __X86__
    if(started) {
        Flurry::Analytics::LogEvent("Import", started);
    } else {
        Flurry::Analytics::EndTimedEvent("Import");
    }
#endif
}

void Analytics::importedSongCount(int count) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["count"] = count;
    Flurry::Analytics::LogEvent("ImportCount", parameters, false);
#endif
}

void Analytics::importedPlaylistCount(int count) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["count"] = count;
    Flurry::Analytics::LogEvent("ImportPlaylistCount", parameters, false);
#endif
}

void Analytics::createPlaylist(QString const& name) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["name"] = name;
    Flurry::Analytics::LogEvent("CreatePlaylist", parameters, false);
#endif
}

void Analytics::deleteAllPlaylists() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("DeleteAllPlaylists", false);
#endif
}

void Analytics::createAlbum(QString const& artistName, QString const& albumName) {
#ifndef __X86__
    Flurry::Map parameters;
    parameters["artistName"] = artistName;
    parameters["albumName"] = albumName;
    Flurry::Analytics::LogEvent("CreateAlbum", parameters, false);
#endif
}

void Analytics::play() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("PlayCmd", false);
#endif
}

void Analytics::pause() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("PauseCmd", false);
#endif
}

void Analytics::stop() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("StopCmd", false);
#endif
}

void Analytics::resume() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("ResumeCmd", false);
#endif
}

void Analytics::rewind() {
#ifndef __X86__
    Flurry::Analytics::LogEvent("RewindCmd", false);
#endif
}

void Analytics::help(int on) {
#ifndef __X86__
    if(on) {
        Flurry::Analytics::LogEvent("Help", true);
    } else {
        Flurry::Analytics::EndTimedEvent("Help");
    }
#endif
}

void Analytics::settings(int on) {
#ifndef __X86__
    if(on) {
        Flurry::Analytics::LogEvent("Settings", true);
    } else {
        Flurry::Analytics::EndTimedEvent("Settings");
    }
#endif
}

void Analytics::buy(int on) {
#ifndef __X86__
    if(on) {
        Flurry::Analytics::LogEvent("Buy", true);
    } else {
        Flurry::Analytics::EndTimedEvent("Buy");
    }
#endif
}

void Analytics::shareSong(QString const& fileName)
{
#ifndef __X86__
    HardwareInfo hwInfo;
    Flurry::Map parameters;
    parameters["pin"] = hwInfo.pin();
    parameters["imei"] = hwInfo.imei();
    parameters["deviceName"] = hwInfo.deviceName();
    parameters["hardwareId"] = hwInfo.hardwareId();
    parameters["meid"] = hwInfo.meid();
    parameters["serial"] = hwInfo.serialNumber();
    parameters["fileName"] = fileName;
    Flurry::Analytics::LogEvent("ShareSong", parameters, false);
#endif
}

void Analytics::purchase(QString const& info) {
#ifndef __X86__
    HardwareInfo hwInfo;
    Flurry::Map parameters;
    parameters["pin"] = hwInfo.pin();
    parameters["imei"] = hwInfo.imei();
    parameters["deviceName"] = hwInfo.deviceName();
    parameters["hardwareId"] = hwInfo.hardwareId();
    parameters["meid"] = hwInfo.meid();
    parameters["serial"] = hwInfo.serialNumber();
    parameters["info"] = info;
    Flurry::Analytics::LogEvent("Purchase", parameters, false);
#endif
}

void Analytics::active(int on) {
#ifndef __X86__
    HardwareInfo hwInfo;
    Flurry::Map parameters;
    parameters["pin"] = hwInfo.pin();
    parameters["imei"] = hwInfo.imei();
    parameters["deviceName"] = hwInfo.deviceName();
    parameters["hardwareId"] = hwInfo.hardwareId();
    parameters["meid"] = hwInfo.meid();
    parameters["serial"] = hwInfo.serialNumber();
    if(on) {
        Flurry::Analytics::LogEvent("Active", parameters, true);
    } else {
        Flurry::Analytics::EndTimedEvent("Active", parameters);
    }
#endif
}

void Analytics::invoke(QString const& source,
                       QString const& target,
                       QString const& action,
                       QString const& mimeType,
                       QUrl const& uri) {
#ifndef __X86__
    HardwareInfo hwInfo;
    Flurry::Map parameters;
    parameters["pin"] = hwInfo.pin();
    parameters["source"] = source;
    parameters["target"] = target;
    parameters["action"] = action;
    parameters["mimeType"] = mimeType;
    parameters["uri"] = uri.toString();
    Flurry::Analytics::LogEvent("Invoke", parameters, false);
#endif
}
