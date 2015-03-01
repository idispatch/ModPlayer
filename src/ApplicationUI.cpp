#include <bb/cascades/Window>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>
#include <bb/cascades/pickers/FilePicker>
#include <bb/cascades/pickers/FilePickerMode>
#include <bb/cascades/pickers/FilePickerSortFlag>
#include <bb/cascades/pickers/FilePickerSortOrder>
#include <bb/cascades/pickers/FilePickerViewMode>
#include <bb/cascades/pickers/FileType>
#include <bb/device/DisplayInfo>
#include <bb/data/DataSource>
#include <bb/system/InvokeManager>
#include <bb/system/SystemToast>
#include <bb/platform/PaymentManager>
#include <bb/ApplicationInfo>

#include "ApplicationUI.hpp"
#include "Analytics.hpp"
#include "Player.hpp"
#include "LCDDisplay.hpp"
#include "LCDDigits.hpp"
#include "SongBasicInfo.hpp"
#include "SongExtendedInfo.hpp"
#include "SongModule.hpp"
#include "SongFormat.hpp"
#include "SongGenre.hpp"
#include "Artist.hpp"
#include "PlaybackConfig.hpp"
#include "Cache.hpp"
#include "Catalog.hpp"
#include "Downloader.hpp"
#include "Playback.hpp"
#include "PatternView.hpp"
#include "VUMeter.hpp"
#include "Playlist.hpp"
#include "NamedPlaylist.hpp"
#include "Album.hpp"
#include "Radio.hpp"
#include "WebImageView.hpp"
#include "AlbumArtView.hpp"
#include "InternetRadio.hpp"
#include "Wallpaper.hpp"
#include "PurchaseStore.hpp"
#include "PlayActionItem.hpp"
#include "PauseActionItem.hpp"
#include "SleepTimer.hpp"
#include "FileUtils.hpp"
#include "FileSystem.hpp"
#include "FileEntry.hpp"

using namespace bb::data;
using namespace bb::cascades;
using namespace bb::system;

const char * ApplicationUI::QmlNamespace = "player";
ApplicationUI* ApplicationUI::static_instance;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
    : QObject(app),
      m_appState(bb::ProcessState::Foreground),
      m_pTranslator(new QTranslator(this)),
      m_purchaseStore(m_settings),
      m_wallpaper(m_settings),
      m_pLocaleHandler(new LocaleHandler(this)),
      m_player(new Player(m_settings, this)),
      m_analytics(new Analytics(this)),
      m_fileSystem(new FileSystem(m_player->filters(), this)),
      m_proximity(new Proximity(this)),
      m_shake(new Shake(this)),
      m_invokeManager(new InvokeManager(this)) {
    static_instance = this;
    m_app = app;
    m_analytics->active(1);
    initScreen();
    initTranslator();
    initTypes();
    initSignals();
    initApp();
    initActiveCover();
    initPlayer();
    initProximity();
    initShake();
    initPurchases();
}

ApplicationUI::~ApplicationUI() {
    if(m_analytics != NULL) {
        m_analytics->active(0);
    }
    static_instance = NULL;
}

void ApplicationUI::updateScreen() {
    bb::cascades::Window * window = Application::instance()->mainWindow();
    if(window != NULL) {
        if(keepScreenAwake()) {
            window->setScreenIdleMode(ScreenIdleMode::KeepAwake);
        } else {
            window->setScreenIdleMode(ScreenIdleMode::Normal);
        }
    }
}

void ApplicationUI::initScreen() {
    bb::device::DisplayInfo display;
    m_screenPixelSize = display.pixelSize();
    updateScreen();
}

int ApplicationUI::screenWidth() const {
    return m_screenPixelSize.width();
}

int ApplicationUI::screenHeight() const {
    return m_screenPixelSize.height();
}

bool ApplicationUI::isFirstLaunch() const {
    return !QFile(FileUtils::joinPath(QDir::homePath(), ".first_launch")).exists();
}

void ApplicationUI::setFirstLaunch(bool value) {
    QFile file(FileUtils::joinPath(QDir::homePath(), ".first_launch"));
    if(value) {
        file.remove();
    } else {
        if(file.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream(&file) << QDateTime::currentDateTime().toString(Qt::ISODate);
            file.close();
        }
    }
    emit isFirstLaunchChanged();
}

int ApplicationUI::maxViewSongs() const {
    return m_settings.value("ui/maxViewSongs", 200).toInt();
}

void ApplicationUI::setMaxViewSongs(int value) {
    if(maxViewSongs() != value)
    {
        m_settings.setValue("ui/maxViewSongs", value);
        emit maxViewSongsChanged();
    }
}

bool ApplicationUI::keepScreenAwake() const {
    return m_settings.value("ui/keepScreenAwake", false).toBool();
}

void ApplicationUI::setKeepScreenAwake(bool value) {
    if(keepScreenAwake() != value)
    {
        m_settings.setValue("ui/keepScreenAwake", value);
        emit keepScreenAwakeChanged();
    }
}

bool ApplicationUI::isForeground() const {
    return m_appState == bb::ProcessState::Foreground;
}

bool ApplicationUI::isExtendedVersion() {
    return m_purchaseStore.purchased();
}

void ApplicationUI::onProcessStateChanged(bb::ProcessState::Type processState) {
    if(m_appState != processState) {
        bool wasForeground = isForeground();
        m_appState = processState;
        if(wasForeground != isForeground()) {
            qDebug() << title() << "is foreground:" << isForeground();
            emit isForeground();
        }
    }
}

ApplicationUI& ApplicationUI::instance() {
    return *static_instance;
}

void ApplicationUI::onPurchaseStateChanged() {
    emit isExtendedVersionChanged();
}

void ApplicationUI::initPurchases() {
    m_purchaseStore.loadLocalPurchases();
    if(!m_purchaseStore.purchased()) {
        m_purchaseStore.loadPurchasesFromStore();
    }
}

void ApplicationUI::initSignals() {
    bool rc;
    rc = QObject::connect(m_app, SIGNAL(aboutToQuit()),
                          this,  SLOT(onAboutToQuit()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_app, SIGNAL(processStateChanged(bb::ProcessState::Type)),
                          this,  SLOT(onProcessStateChanged(bb::ProcessState::Type)));
    Q_ASSERT(rc);

    rc = QObject::connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
                          this,            SLOT(onInvoked(const bb::system::InvokeRequest&)));
    Q_ASSERT(rc);

    rc = QObject::connect(&m_purchaseStore, SIGNAL(purchasedChanged()),
                          this,            SLOT(onPurchaseStateChanged()));

    Q_ASSERT(rc);

    Q_UNUSED(rc);
}

void ApplicationUI::onInvoked(const InvokeRequest& request) {
#if 0
    qDebug() << "ApplicationUI::onInvoke: action" << request.action()
             << "target" << request.target()
             << "uri" << request.uri()
             << "mime" << request.mimeType();
#endif
    m_analytics->invoke(request.source().installId(),
                        request.target(),
                        request.action(),
                        request.mimeType(),
                        request.uri());
    if(request.action() == "bb.action.OPEN" ||
       request.action() == "bb.action.VIEW") {
        QString fileName = request.uri().toString();
        if(fileName.startsWith("file://")) {
            fileName.remove(0, 7);
            m_player->playLocalSong(fileName);
        } else {
            qDebug() << "Invalid invoke URI" << request.uri();
        }
    } else {
        qDebug() << "Invalid request action" << request.action();
    }
}

void ApplicationUI::saveWallpaperSettings() {
    m_wallpaper.save(m_settings);
    m_settings.sync();
}

void ApplicationUI::saveSettings() {
    m_settings.sync();
}

void ApplicationUI::onAboutToQuit() {
    saveWallpaperSettings();
    LCDDigits::finalize();
    if(m_player != 0) {
        m_player->setParent(0);
        delete m_player;
        m_player = 0;
    }
    saveSettings();
}

void ApplicationUI::initTypes() {
    const int versionMajor = 1, versionMinor = 0;

    qRegisterMetaType<QVector<QString> >("QVector<QString>");

    DataSource::registerQmlTypes();
    qmlRegisterType<QTimer>(QmlNamespace, versionMajor, versionMinor, "QTimer");

    qmlRegisterType<bb::platform::PaymentManager>("bb.platform", 1, 0, "PaymentManager");

    const char * PICKER_NAMESPACE = "bb.cascades.pickers";
    qmlRegisterType<bb::cascades::pickers::FilePicker>(PICKER_NAMESPACE, 1, 0, "FilePicker");
    qmlRegisterUncreatableType<bb::cascades::pickers::FilePickerMode>(PICKER_NAMESPACE, 1, 0, "FilePickerMode", "");
    qmlRegisterUncreatableType<bb::cascades::pickers::FilePickerSortFlag>(PICKER_NAMESPACE, 1, 0, "FilePickerSortFlag", "");
    qmlRegisterUncreatableType<bb::cascades::pickers::FilePickerSortOrder>(PICKER_NAMESPACE, 1, 0, "FilePickerSortOrder", "");
    qmlRegisterUncreatableType<bb::cascades::pickers::FileType>(PICKER_NAMESPACE, 1, 0, "FileType", "");
    qmlRegisterUncreatableType<bb::cascades::pickers::FilePickerViewMode>(PICKER_NAMESPACE, 1, 0, "FilePickerViewMode", "");

    const char * MULTIMEDIA_NAMESPACE = "bb.multimedia";
    qmlRegisterUncreatableType<bb::multimedia::MediaState>(MULTIMEDIA_NAMESPACE, 1, 0, "MediaState", "");
    qmlRegisterUncreatableType<bb::multimedia::BufferStatus>(MULTIMEDIA_NAMESPACE, 1, 0, "BufferStatus", "");

    qmlRegisterType<PlayActionItem>(QmlNamespace, versionMajor, versionMinor, "PlayActionItem");
    qmlRegisterType<PauseActionItem>(QmlNamespace, versionMajor, versionMinor, "PauseActionItem");
    qmlRegisterType<WebImageView>(QmlNamespace, versionMajor, versionMinor, "WebImageView");
    qmlRegisterType<AlbumArtView>(QmlNamespace, versionMajor, versionMinor, "AlbumArtView");
    qmlRegisterType<LCDDisplay>(QmlNamespace, versionMajor, versionMinor, "LCDDisplay");
    qmlRegisterType<LCDDigits>(QmlNamespace, versionMajor, versionMinor, "LCDDigits");
    qmlRegisterType<VUMeter>(QmlNamespace, versionMajor, versionMinor, "VUMeter");
    qmlRegisterType<PatternView>(QmlNamespace, versionMajor, versionMinor, "PatternView");

    qmlRegisterUncreatableType<Player>(QmlNamespace, versionMajor, versionMinor, "Player", "");
    qmlRegisterUncreatableType<Catalog>(QmlNamespace, versionMajor, versionMinor, "Catalog", "");
    qmlRegisterUncreatableType<Cache>(QmlNamespace, versionMajor, versionMinor, "Cache", "");
    qmlRegisterUncreatableType<Downloader>(QmlNamespace, versionMajor, versionMinor, "Downloader", "");
    qmlRegisterUncreatableType<InternetRadio>(QmlNamespace, versionMajor, versionMinor, "InternetRadio", "");

    qmlRegisterUncreatableType<SongModule>(QmlNamespace, versionMajor, versionMinor, "Module", "");
    qmlRegisterUncreatableType<Playback>(QmlNamespace, versionMajor, versionMinor, "Playback", "");
    qmlRegisterUncreatableType<PlaybackConfig>(QmlNamespace, versionMajor, versionMinor, "PlaybackConfig", "");

    qmlRegisterUncreatableType<SongFormat>(QmlNamespace, versionMajor, versionMinor, "SongFormat", "");
    qmlRegisterUncreatableType<SongGenre>(QmlNamespace, versionMajor, versionMinor, "SongGenre", "");
    qmlRegisterUncreatableType<SongBasicInfo>(QmlNamespace, versionMajor, versionMinor, "SongBasicInfo", "");
    qmlRegisterUncreatableType<SongExtendedInfo>(QmlNamespace, versionMajor, versionMinor, "SongExtendedInfo", "");
    qmlRegisterUncreatableType<Artist>(QmlNamespace, versionMajor, versionMinor, "Artist", "");
    qmlRegisterUncreatableType<Analytics>(QmlNamespace, versionMajor, versionMinor, "Analytics", "");

    qmlRegisterUncreatableType<Playlist>(QmlNamespace, versionMajor, versionMinor, "Playlist", "");
    qmlRegisterUncreatableType<NamedPlaylist>(QmlNamespace, versionMajor, versionMinor, "NamedPlaylist", "");
    qmlRegisterUncreatableType<Album>(QmlNamespace, versionMajor, versionMinor, "Album", "");
    qmlRegisterUncreatableType<Radio>(QmlNamespace, versionMajor, versionMinor, "Radio", "");
    qmlRegisterUncreatableType<Wallpaper>(QmlNamespace, versionMajor, versionMinor, "Wallpaper", "");
    qmlRegisterUncreatableType<FileSystem>(QmlNamespace, versionMajor, versionMinor, "FileSystem", "");
    qmlRegisterUncreatableType<FileEntry>(QmlNamespace, versionMajor, versionMinor, "FileEntry", "");
    qmlRegisterUncreatableType<PurchaseStore>(QmlNamespace, versionMajor, versionMinor, "PurchaseStore", "");
    qmlRegisterUncreatableType<SleepTimer>(QmlNamespace, versionMajor, versionMinor, "SleepTimer", "");
}

void ApplicationUI::initApp() {
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    if (!qml->hasErrors())
    {
        qml->setContextProperty("app", this);
        AbstractPane *appPage = qml->createRootObject<AbstractPane>();
        if (appPage)
        {
            Application::instance()->setScene(appPage);
        }
    }
}

void ApplicationUI::initActiveCover() {
    QmlDocument *qml = QmlDocument::create("asset:///Cover.qml").parent(this);
    if (!qml->hasErrors())
    {
        qml->setContextProperty("app", this);
        Container *coverContainer = qml->createRootObject<Container>();
        SceneCover *sceneCover = SceneCover::create().content(coverContainer);
        Application::instance()->setCover(sceneCover);
    }
}

void ApplicationUI::initPlayer() {
    bool rc;
    rc = QObject::connect(m_player, SIGNAL(catalogChanged()),
                          this,     SLOT(onCatalogChanged()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_player, SIGNAL(cacheChanged()),
                          this,     SLOT(onCacheChanged()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void ApplicationUI::initProximity() {
    bool rc;
    rc = QObject::connect(m_proximity, SIGNAL(closeProximity()),
                          this,        SLOT(onCloseProximity()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    m_proximity->setActive(isProximitySensorEnabled());
}

void ApplicationUI::initShake() {
    bool rc;
    rc = QObject::connect(m_shake, SIGNAL(shakeDetected()),
                          this,    SLOT(onShakeDetected()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    m_shake->setActive(isShakeSensorEnabled());
}

void ApplicationUI::initTranslator() {
    bool rc;
    rc = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()),
                          this,             SLOT(onSystemLanguageChanged()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
    onSystemLanguageChanged();
}

void ApplicationUI::onSystemLanguageChanged() {
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    QString localeString = QLocale().name();
    QString fileName = QString("ModPlayer_%1").arg(localeString);
    qDebug() << "Using locale file name " << fileName;
    if (m_pTranslator->load(fileName, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

void ApplicationUI::onCatalogChanged() {
    emit catalogChanged();
}

void ApplicationUI::onCacheChanged() {
    emit cacheChanged();
}

void ApplicationUI::onCloseProximity() {
    if(isProximitySensorEnabled()) {
        m_player->playNext();
    }
}

bool ApplicationUI::isProximitySensorEnabled() const {
    return m_settings.value("ui/proximity", false).toBool();
}

void ApplicationUI::setProximitySensorEnabled(bool value) {
    if(isProximitySensorEnabled() != value) {
        m_settings.setValue("ui/proximity", value);
        m_proximity->setActive(value);
        emit proximitySensorEnabledChanged();
    }
}

void ApplicationUI::onShakeDetected() {
    if(isShakeSensorEnabled()) {
        m_player->playNext();
    }
}

bool ApplicationUI::isShakeSensorEnabled() const {
    return m_settings.value("ui/shake", false).toBool();
}

void ApplicationUI::setShakeSensorEnabled(bool value) {
    if(isShakeSensorEnabled() != value) {
        m_settings.setValue("ui/shake", value);
        m_shake->setActive(value);
        emit shakeSensorEnabledChanged();
    }
}

QString ApplicationUI::title() const {
    using namespace bb;
    ApplicationInfo info;
    return info.title();
}

QString ApplicationUI::version() const {
    using namespace bb;
    ApplicationInfo info;
    return info.version();
}

FileSystem * ApplicationUI::fileSystem() {
    return m_fileSystem;
}

Player * ApplicationUI::player() const {
    return m_player;
}

Catalog * ApplicationUI::catalog() const {
    return m_player->catalog();
}

Cache * ApplicationUI::cache() const {
    return m_player->cache();
}

Wallpaper * ApplicationUI::wallpaper() {
    return &m_wallpaper;
}

PurchaseStore * ApplicationUI::store() {
    return &m_purchaseStore;
}

Analytics * ApplicationUI::analytics() const {
    return m_analytics;
}

void ApplicationUI::emailAuthor() {
    analytics()->email();

    QUrl url = QUrl("mailto:oleg@kosenkov.ca");
    QList<QPair<QString, QString> > query;
    query << QPair<QString, QString>("subject", title())
          << QPair<QString, QString>("body", tr("Hello, ModPlayer Author!"));
    url.setQueryItems(query);

    InvokeRequest request;
    request.setTarget("sys.pim.uib.email.hybridcomposer");
    request.setAction("bb.action.SENDEMAIL");
    request.setMimeType("text/plain");
    request.setUri(url);
    m_invokeManager->invoke(request);
}

void ApplicationUI::twit() {
    analytics()->twit();

    InvokeRequest request;
    request.setTarget("Twitter");
    request.setAction("bb.action.SHARE");
    request.setMimeType("text/plain");
    request.setUri(QUrl("data://"));
    QString message(tr("Enjoying the ModPlayer #app on my #BlackBerry 10 device, check it out at #BlackBerry World #BlackBerry10!"));
    request.setData(message.toUtf8());
    m_invokeManager->invoke(request);
}

void ApplicationUI::bbm() {
    analytics()->bbm();

    InvokeRequest request;
    request.setTarget("sys.bbm.sharehandler");
    request.setAction("bb.action.SHARE");
    request.setMimeType("text/plain");
    QString message(tr("Enjoying the ModPlayer app on my BlackBerry 10 device, check it out at BlackBerry World!"));
    request.setData(message.toUtf8());
    m_invokeManager->invoke(request);
}

void ApplicationUI::appWorld() {
    analytics()->appWorld();

    InvokeRequest request;
    request.setTarget("sys.appworld");
    request.setAction("bb.action.OPEN");
    request.setMimeType("text/plain");
    if(m_purchaseStore.isModPlayerPlusEdition()) {
        request.setUri(QUrl("appworld://content/59950203"));
    } else {
        request.setUri(QUrl("appworld://content/35007887"));
    }
    m_invokeManager->invoke(request);
}

void ApplicationUI::shareSong(QString const& fileName) {
    if(isExtendedVersion()) {
        analytics()->shareSong(fileName);
        QUrl url("file://" + fileName);
        InvokeRequest request;
        request.setTarget("sys.pim.uib.email.hybridcomposer");
        request.setAction("bb.action.SHARE");
        request.setUri(url);
        m_invokeManager->invoke(request);
    }
}

void ApplicationUI::pleaseBuy() {
    if(!isExtendedVersion()) {
        SystemToast toast;
        toast.setBody(tr("This feature is available in ModPlayer Plus."));
        toast.setModality(SystemUiModality::Application);
        toast.setPosition(SystemUiPosition::MiddleCenter);
        toast.button()->setLabel("Ok");
        toast.exec();
    }
}
