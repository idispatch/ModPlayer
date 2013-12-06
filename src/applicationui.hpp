#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QVariant>
#include <QUrl>
#include <QSettings>

namespace bb
{
    namespace cascades
    {
        class Application;
        class LocaleHandler;
    }
    namespace system
    {
        class InvokeManager;
        class InvokeRequest;
    }
}

class QTranslator;
class Analytics;
class Player;
class Catalog;
class Cache;

class ApplicationUI : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isExtendedVersion READ isExtendedVersion NOTIFY isExtendedVersionChanged FINAL)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged FINAL)
    Q_PROPERTY(Player* player READ player NOTIFY playerChanged FINAL)
    Q_PROPERTY(Catalog* catalog READ catalog NOTIFY catalogChanged FINAL)
    Q_PROPERTY(Cache* cache READ cache NOTIFY cacheChanged FINAL)
    Q_PROPERTY(Analytics* analytics READ analytics NOTIFY analyticsChanged FINAL)
public:
    ApplicationUI(bb::cascades::Application *app);
    ~ApplicationUI();

    bool isExtendedVersion() const;

    QString version() const;
    Player * player() const;
    Catalog * catalog() const;
    Cache * cache() const;
    Analytics * analytics() const;

    Q_INVOKABLE void emailAuthor();
    Q_INVOKABLE void twit();
    Q_INVOKABLE void bbm();
    Q_INVOKABLE void appWorld();

    static const char * QmlNamespace;
Q_SIGNALS:
    void isExtendedVersionChanged();
    void versionChanged();
    void playerChanged();
    void catalogChanged();
    void cacheChanged();
    void analyticsChanged();
private slots:
    void onInvoked(const bb::system::InvokeRequest& invoke);
    void onSystemLanguageChanged();
    void onCatalogChanged();
    void onCacheChanged();
    void onAboutToQuit();
private:
    void initSignals();
    void initTypes();
    void initApp();
    void initActiveCover();
    void initPlayer();
    void initTranslator();
private:
    static ApplicationUI * instance;
    QSettings m_settings;
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    bb::cascades::Application * m_app;
    Player * m_player;
    Analytics * m_analytics;
    bb::system::InvokeManager * m_invokeManager;
};

#endif
