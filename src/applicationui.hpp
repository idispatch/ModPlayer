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
}

class QTranslator;
class Player;
class Catalog;
class Cache;

class ApplicationUI : public QObject {
    Q_OBJECT
    Q_PROPERTY(Player* player READ player NOTIFY playerChanged FINAL)
    Q_PROPERTY(Catalog* catalog READ catalog NOTIFY catalogChanged FINAL)
    Q_PROPERTY(Cache* cache READ cache NOTIFY cacheChanged FINAL)
public:
    ApplicationUI(bb::cascades::Application *app);
    ~ApplicationUI();

    Player * player() const;
    Catalog * catalog() const;
    Cache * cache() const;

    Q_INVOKABLE QUrl getIconPath(QVariant value) const;

    static const char * QmlNamespace;
Q_SIGNALS:
    void playerChanged();
    void catalogChanged();
    void cacheChanged();
private slots:
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
    QSettings m_settings;
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    bb::cascades::Application * m_app;
    Player * m_player;
};

#endif
