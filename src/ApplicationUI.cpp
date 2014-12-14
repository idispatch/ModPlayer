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
#include <bb/data/DataSource>
#include <bb/system/InvokeManager>
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

using namespace bb::data;
using namespace bb::cascades;
using namespace bb::system;

const char * ApplicationUI::QmlNamespace = "player";
ApplicationUI* ApplicationUI::s_instance;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
    : QObject(app),
      m_appState(bb::ProcessState::Foreground),
      m_pTranslator(new QTranslator(this)),
      m_pLocaleHandler(new LocaleHandler(this)),
      m_player(new Player(m_settings, this)),
      m_analytics(new Analytics(this)),
      m_invokeManager(new InvokeManager(this)) {
    s_instance = this;
    m_app = app;
    m_analytics->active(1);
    initTranslator();
    initTypes();
    initSignals();
    initApp();
    initActiveCover();
    initPlayer();
}

ApplicationUI::~ApplicationUI() {
    if(m_analytics != NULL) {
        m_analytics->active(0);
    }
    s_instance = NULL;
}

bool ApplicationUI::isForeground() const {
    return m_appState == bb::ProcessState::Foreground;
}


void ApplicationUI::onProcessStateChanged(bb::ProcessState::Type processState) {
    if(m_appState != processState) {
        bool wasForeground = isForeground();
        m_appState = processState;
        if(wasForeground != isForeground()) {
            qDebug() << "ModPlayer is foreground:" << isForeground();
            emit isForeground();
        }
    }
}

ApplicationUI& ApplicationUI::instance() {
    return *s_instance;
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

void ApplicationUI::onAboutToQuit() {
    LCDDigits::finalize();
    if(m_player != 0) {
        m_player->setParent(0);
        delete m_player;
        m_player = 0;
    }
    m_settings.sync();
}

void ApplicationUI::initTypes() {
    const int versionMajor = 1, versionMinor = 0;

    qRegisterMetaType<QVector<QString> >("QVector<QString>");

    DataSource::registerQmlTypes();
    qmlRegisterType<QTimer>(QmlNamespace, versionMajor, versionMinor, "QTimer");

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

    qRegisterMetaType<Analytics*>();
    qRegisterMetaType<Artist*>();
    qRegisterMetaType<Cache*>();
    qRegisterMetaType<Catalog*>();
    qRegisterMetaType<Downloader*>();
    qRegisterMetaType<NamedItem*>();
    qRegisterMetaType<ItemGroupBase*>();
    qRegisterMetaType<LCDDigits*>();
    qRegisterMetaType<Playback*>();
    qRegisterMetaType<PlaybackConfig*>();
    qRegisterMetaType<Player*>();
    qRegisterMetaType<SongBasicInfo*>();
    qRegisterMetaType<SongExtendedInfo*>();
    qRegisterMetaType<SongModule*>();
    qRegisterMetaType<SongFormat*>();
    qRegisterMetaType<SongGenre*>();
    qRegisterMetaType<PatternView*>();
    qRegisterMetaType<VUMeter*>();
    qRegisterMetaType<Radio*>();

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

QString ApplicationUI::version() const {
    using namespace bb;
    ApplicationInfo info;
    return info.version();
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

Analytics * ApplicationUI::analytics() const {
    return m_analytics;
}

void ApplicationUI::emailAuthor() {
    analytics()->email();

    QUrl url = QUrl("mailto:oleg@kosenkov.ca");
    QList<QPair<QString, QString> > query;
    query << QPair<QString, QString>("subject", "ModPlayer")
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
    request.setUri(QUrl("appworld://content/35007887"));
    m_invokeManager->invoke(request);
}
