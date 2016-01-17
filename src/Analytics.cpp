#include "Analytics.hpp"
#include "Flurry/Flurry.h"

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

void Analytics::appWorld() {
    Flurry::Analytics::LogEvent("AppWorld", false);
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
    logError("DownloadFailed", QString("Module %1").arg(moduleId));
}

void Analytics::logError(char const * error,
						 QString const& message) {
    Flurry::Analytics::LogError(error, message);
}

void Analytics::showPage(QString const& name) {
    Flurry::Map parameters;
    parameters["Page"] = name;
    Flurry::Analytics::LogEvent("ShowPage", parameters, false);
}

void Analytics::playRadio(QString const& name) {
    Flurry::Map parameters;
    parameters["Channel"] = name;
    Flurry::Analytics::LogEvent("PlayRadio", parameters, false);
}

void Analytics::selectRadio(QString const& name) {
    Flurry::Map parameters;
    parameters["Channel"] = name;
    Flurry::Analytics::LogEvent("SelectRadio", parameters, false);
}

void Analytics::view(int moduleId, QString const& fileName) {
    logModuleEvent("View", moduleId, fileName);
}

void Analytics::search(QString const& query) {
    Flurry::Map parameters;
    parameters["Query"] = query;
    Flurry::Analytics::LogEvent("Search", parameters, false);
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

void Analytics::saveCache(QString const& from, QString const& to) {
    Flurry::Map parameters;
    parameters["fromFile"] = from;
    parameters["toFile"] = to;
    Flurry::Analytics::LogEvent("SaveCache", parameters, false);
}

void Analytics::exportCache(int numFiles, int numCopiedFiles) {
    Flurry::Map parameters;
    parameters["cacheFiles"] = numFiles;
    parameters["copiedFiles"] = numCopiedFiles;
    Flurry::Analytics::LogEvent("ExportCache", parameters, false);
}

void Analytics::exportMp3(QString const& from, QString const& to) {
    Flurry::Map parameters;
    parameters["fromFile"] = from;
    parameters["toFile"] = to;
    Flurry::Analytics::LogEvent("ExportMp3", parameters, false);
}

void Analytics::importSongs(bool started) {
    if(started) {
        Flurry::Analytics::LogEvent("Import", started);
    } else {
        Flurry::Analytics::EndTimedEvent("Import");
    }
}

void Analytics::importedSongCount(int count) {
    Flurry::Map parameters;
    parameters["count"] = count;
    Flurry::Analytics::LogEvent("ImportCount", parameters, false);
}

void Analytics::importedPlaylistCount(int count) {
    Flurry::Map parameters;
    parameters["count"] = count;
    Flurry::Analytics::LogEvent("ImportPlaylistCount", parameters, false);
}

void Analytics::createPlaylist(QString const& name) {
    Flurry::Map parameters;
    parameters["name"] = name;
    Flurry::Analytics::LogEvent("CreatePlaylist", parameters, false);
}

void Analytics::deleteAllPlaylists() {
    Flurry::Analytics::LogEvent("DeleteAllPlaylists", false);
}

void Analytics::createAlbum(QString const& artistName, QString const& albumName) {
    Flurry::Map parameters;
    parameters["artistName"] = artistName;
    parameters["albumName"] = albumName;
    Flurry::Analytics::LogEvent("CreateAlbum", parameters, false);
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

void Analytics::buy(int on) {
    if(on) {
        Flurry::Analytics::LogEvent("Buy", true);
    } else {
        Flurry::Analytics::EndTimedEvent("Buy");
    }
}

void Analytics::shareSong(QString const& fileName)
{
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
}

void Analytics::purchase(QString const& info) {
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
}

void Analytics::active(int on) {
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
}

void Analytics::invoke(QString const& source,
                       QString const& target,
                       QString const& action,
                       QString const& mimeType,
                       QUrl const& uri) {
    HardwareInfo hwInfo;
    Flurry::Map parameters;
    parameters["pin"] = hwInfo.pin();
    parameters["source"] = source;
    parameters["target"] = target;
    parameters["action"] = action;
    parameters["mimeType"] = mimeType;
    parameters["uri"] = uri.toString();
    Flurry::Analytics::LogEvent("Invoke", parameters, false);
}
