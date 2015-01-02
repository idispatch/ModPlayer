#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QVariant>
#include <QUrl>
#include <QSettings>
#include <bb/ProcessState>
#include "Wallpaper.hpp"

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
class Wallpaper;

class ApplicationUI : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(ApplicationUI)
    Q_PROPERTY(bool isExtendedVersion READ isExtendedVersion NOTIFY isExtendedVersionChanged FINAL)
    Q_PROPERTY(bool isForeground READ isForeground NOTIFY isForegroundChanged FINAL)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged FINAL)
    Q_PROPERTY(Player* player READ player NOTIFY playerChanged FINAL)
    Q_PROPERTY(Catalog* catalog READ catalog NOTIFY catalogChanged FINAL)
    Q_PROPERTY(Cache* cache READ cache NOTIFY cacheChanged FINAL)
    Q_PROPERTY(Wallpaper* wallpaper READ wallpaper NOTIFY wallpaperChanged FINAL)
    Q_PROPERTY(Analytics* analytics READ analytics NOTIFY analyticsChanged FINAL)
public:
    ApplicationUI(bb::cascades::Application *app);
    ~ApplicationUI();

    static ApplicationUI& instance();
    bool isExtendedVersion() const;

    bool isForeground() const;

    QString version() const;
    Player * player() const;
    Catalog * catalog() const;
    Cache * cache() const;
    Analytics * analytics() const;
    Wallpaper * wallpaper();

    Q_INVOKABLE void emailAuthor();
    Q_INVOKABLE void twit();
    Q_INVOKABLE void bbm();
    Q_INVOKABLE void appWorld();

    static const char * QmlNamespace;
Q_SIGNALS:
    void isExtendedVersionChanged();
    void isForegroundChanged();
    void versionChanged();
    void playerChanged();
    void catalogChanged();
    void cacheChanged();
    void wallpaperChanged();
    void analyticsChanged();
private slots:
    void onInvoked(const bb::system::InvokeRequest& invoke);
    void onSystemLanguageChanged();
    void onCatalogChanged();
    void onCacheChanged();
    void onAboutToQuit();
    void onProcessStateChanged(bb::ProcessState::Type);
private:
    void initSignals();
    void initTypes();
    void initApp();
    void initActiveCover();
    void initPlayer();
    void initTranslator();
    void saveWallpaperSettings();
private:
    static ApplicationUI * s_instance;
    bb::ProcessState::Type m_appState;
    QSettings m_settings;
    QTranslator* m_pTranslator;
    Wallpaper m_wallpaper;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    bb::cascades::Application * m_app;
    Player * m_player;
    Analytics * m_analytics;
    bb::system::InvokeManager * m_invokeManager;
};

#endif
