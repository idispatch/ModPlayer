#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>

#include <bb/data/DataSource>
#include <bb/system/InvokeManager>

#include "applicationui.hpp"
#include "Player.hpp"
#include "LCDDisplay.hpp"
#include "LCDDigits.hpp"
#include "SongBasicInfo.hpp"
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
      m_player(new Player(m_settings, this)) {
    m_app = app;
    initTypes();
    initSignals();
    initApp();
    initActiveCover();
    initPlayer();
}

ApplicationUI::~ApplicationUI() {
}

void ApplicationUI::initSignals() {
    bool rc;
    rc = QObject::connect(m_app,
                          SIGNAL(aboutToQuit()),
                          this,
                          SLOT(onAboutToQuit()));
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
    qmlRegisterUncreatableType<SongInfo>(QmlNamespace, versionMajor, versionMinor, "SongInfo", "");
    qmlRegisterUncreatableType<Artist>(QmlNamespace, versionMajor, versionMinor, "Artist", "");
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
    rc = QObject::connect(m_player,
                          SIGNAL(catalogChanged()),
                          this,
                          SLOT(onCatalogChanged()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_player,
                          SIGNAL(cacheChanged()),
                          this,
                          SLOT(onCacheChanged()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void ApplicationUI::initTranslator() {
    bool rc;
    rc = QObject::connect(m_pLocaleHandler,
                          SIGNAL(systemLanguageChanged()),
                          this,
                          SLOT(onSystemLanguageChanged()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
    onSystemLanguageChanged();
}

void ApplicationUI::onSystemLanguageChanged() {
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    QString localeString = QLocale().name();
    QString fileName = QString("ModPlayer_%versionMinor").arg(localeString);
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

void ApplicationUI::emailAuthor() {
    qDebug() << "Sending email to author";

    QUrl url = QUrl("mailto:oleg@kosenkov.ca");
    QList<QPair<QString, QString> > query;
    query << QPair<QString, QString>("subject", "ModPlayer")
          << QPair<QString, QString>("body", "Hello, ModPlayer Author!");
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
    InvokeManager invokeManager;
    InvokeRequest request;
    request.setTarget("Twitter");
    request.setAction("bb.action.SHARE");
    request.setMimeType("text/plain");
    request.setUri(QUrl("data://"));
    request.setData("Enjoying the ModPlayer #app on my #BlackBerry 10 device, check it out at #BlackBerry World #BlackBerry10!");
    invokeManager.invoke(request);
}

void ApplicationUI::bbm() {
    InvokeManager invokeManager;
    InvokeRequest request;
    request.setTarget("sys.bbm.sharehandler");
    request.setAction("bb.action.SHARE");
    request.setMimeType("text/plain");
    request.setData("Enjoying the ModPlayer app on my BlackBerry 10 device, check it out at BlackBerry World!");
    invokeManager.invoke(request);
}
