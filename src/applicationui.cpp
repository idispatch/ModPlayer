#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>

#include "applicationui.hpp"
#include "Player.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
    : QObject(app),
      m_pTranslator(new QTranslator(this)),
      m_pLocaleHandler(new LocaleHandler(this)),
      m_player(new Player(this)) {
    qmlRegisterUncreatableType<Player>("player", 1, 0, "Player", "");
    m_app = app;
    initApp();
    initActiveCover();
    initPlayer();
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
    rc = connect(m_player,
                 SIGNAL(catalogChanged()),
                 this,
                 SLOT(onCatalogChanged()));
    Q_ASSERT(rc);

    rc = connect(m_player,
                 SIGNAL(cacheChanged()),
                 this,
                 SLOT(onCacheChanged()));
    Q_ASSERT(rc);
}

void ApplicationUI::initTranslator() {
    bool rc;
    rc = connect(m_pLocaleHandler,
                 SIGNAL(systemLanguageChanged()),
                 this,
                 SLOT(onSystemLanguageChanged()));
    Q_ASSERT(rc);
    onSystemLanguageChanged();
}

void ApplicationUI::onSystemLanguageChanged() {
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    QString locale_string = QLocale().name();
    QString file_name = QString("ZipTest_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
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
