#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>

#include <bb/data/DataSource>
#include <bb/system/InvokeManager>

#include "applicationui.hpp"
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
#include "ModPlayback.hpp"

using namespace bb::data;
using namespace bb::cascades;
using namespace bb::system;

const char * ApplicationUI::QmlNamespace = "player";

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
    : QObject(app),
      m_pTranslator(new QTranslator(this)),
      m_pLocaleHandler(new LocaleHandler(this)),
      m_player(new Player(m_settings, this)),
      m_analytics(new Analytics(this)) {
    m_app = app;
    m_analytics->active(1);
    initTypes();
    initSignals();
    initApp();
    initActiveCover();
    initPlayer();
}

ApplicationUI::~ApplicationUI() {
    m_analytics->active(0);
}

void ApplicationUI::initSignals() {
    bool rc;
    rc = QObject::connect(m_app, SIGNAL(aboutToQuit()),
                          this,  SLOT(onAboutToQuit()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void ApplicationUI::onAboutToQuit() {
    m_settings.setValue("library/version", 1);
    LCDDigits::finalize();
    if(m_player != 0) {
        m_player->setParent(0);
        delete m_player;
        m_player = 0;
    }
    m_settings.sync();
}

void ApplicationUI::initTypes() {
    DataSource::registerQmlTypes();

    qRegisterMetaType<Analytics*>();
    qRegisterMetaType<Artist*>();
    qRegisterMetaType<Cache*>();
    qRegisterMetaType<Catalog*>();
    qRegisterMetaType<Downloader*>();
    qRegisterMetaType<ItemGroupBase*>();
    qRegisterMetaType<LCDDigits*>();
    qRegisterMetaType<ModPlayback*>();
    qRegisterMetaType<PlaybackConfig*>();
    qRegisterMetaType<Player*>();
    qRegisterMetaType<SongBasicInfo*>();
    qRegisterMetaType<SongExtendedInfo*>();
    qRegisterMetaType<SongModule*>();
    qRegisterMetaType<SongFormat*>();
    qRegisterMetaType<SongGenre*>();

    const int versionMajor = 1, versionMinor = 0;

    qmlRegisterUncreatableType<Player>(QmlNamespace, versionMajor, versionMinor, "Player", "");
    qmlRegisterType<LCDDisplay>(QmlNamespace, versionMajor, versionMinor, "LCDDisplay");
    qmlRegisterType<LCDDigits>(QmlNamespace, versionMajor, versionMinor, "LCDDigits");

    qmlRegisterUncreatableType<Catalog>(QmlNamespace, versionMajor, versionMinor, "Catalog", "");
    qmlRegisterUncreatableType<Cache>(QmlNamespace, versionMajor, versionMinor, "Cache", "");
    qmlRegisterUncreatableType<Downloader>(QmlNamespace, versionMajor, versionMinor, "Downloader", "");

    qmlRegisterUncreatableType<SongModule>(QmlNamespace, versionMajor, versionMinor, "Module", "");
    qmlRegisterUncreatableType<ModPlayback>(QmlNamespace, versionMajor, versionMinor, "Playback", "");
    qmlRegisterUncreatableType<PlaybackConfig>(QmlNamespace, versionMajor, versionMinor, "PlaybackConfig", "");

    qmlRegisterUncreatableType<SongFormat>(QmlNamespace, versionMajor, versionMinor, "SongFormat", "");
    qmlRegisterUncreatableType<SongGenre>(QmlNamespace, versionMajor, versionMinor, "SongGenre", "");
    qmlRegisterUncreatableType<SongBasicInfo>(QmlNamespace, versionMajor, versionMinor, "SongBasicInfo", "");
    qmlRegisterUncreatableType<SongExtendedInfo>(QmlNamespace, versionMajor, versionMinor, "SongExtendedInfo", "");
    qmlRegisterUncreatableType<Artist>(QmlNamespace, versionMajor, versionMinor, "Artist", "");
    qmlRegisterUncreatableType<Analytics>(QmlNamespace, versionMajor, versionMinor, "Analytics", "");
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

    InvokeManager invokeManager;
    InvokeRequest request;
    request.setTarget("sys.pim.uib.email.hybridcomposer");
    request.setAction("bb.action.SENDEMAIL");
    request.setMimeType("text/plain");
    request.setUri(url);
    invokeManager.invoke(request);
}

void ApplicationUI::twit() {
    analytics()->twit();

    InvokeManager invokeManager;
    InvokeRequest request;
    request.setTarget("Twitter");
    request.setAction("bb.action.SHARE");
    request.setMimeType("text/plain");
    request.setUri(QUrl("data://"));
    QString message(tr("Enjoying the ModPlayer #app on my #BlackBerry 10 device, check it out at #BlackBerry World #BlackBerry10!"));
    request.setData(message.toUtf8());
    invokeManager.invoke(request);
}

void ApplicationUI::bbm() {
    analytics()->bbm();

    InvokeManager invokeManager;
    InvokeRequest request;
    request.setTarget("sys.bbm.sharehandler");
    request.setAction("bb.action.SHARE");
    request.setMimeType("text/plain");
    QString message(tr("Enjoying the ModPlayer app on my BlackBerry 10 device, check it out at BlackBerry World!"));
    request.setData(message.toUtf8());
    invokeManager.invoke(request);
}

void ApplicationUI::appWorld() {
    analytics()->appWorld();

    InvokeManager invokeManager;
    InvokeRequest request;
    request.setTarget("sys.appworld");
    request.setAction("bb.action.OPEN");
    request.setMimeType("text/plain");
    request.setUri(QUrl("appworld://content/35007887"));
    invokeManager.invoke(request);
}
