#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

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
}

void ApplicationUI::initApp() {
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    //QmlDocument *qml = QmlDocument::create("asset:///FormatsList.qml").parent(this);
    if (!qml->hasErrors()) {
        qml->setContextProperty("app", this);
        AbstractPane *appPage = qml->createRootObject<AbstractPane>();
        if (appPage) {
            Application::instance()->setScene(appPage);
        }
    }
}

void ApplicationUI::initTranslator() {
    if(!QObject::connect(m_pLocaleHandler,
                         SIGNAL(systemLanguageChanged()),
                         this,
                         SLOT(onSystemLanguageChanged()))) {
        qWarning() << "Recovering from a failed connect()";
    }
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

Player * ApplicationUI::player() const {
    return m_player;
}
