#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

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
public:
    ApplicationUI(bb::cascades::Application *app);

    Q_PROPERTY(Player* player READ player NOTIFY playerChanged FINAL)
    Q_PROPERTY(Catalog* catalog READ catalog NOTIFY catalogChanged FINAL)
    Q_PROPERTY(Cache* cache READ cache NOTIFY cacheChanged FINAL)

    Player * player() const;
    Catalog * catalog() const;
    Cache * cache() const;
Q_SIGNALS:
    void playerChanged();
    void catalogChanged();
    void cacheChanged();
private slots:
    void onSystemLanguageChanged();
    void onCatalogChanged();
    void onCacheChanged();
private:
    void initTypes();
    void initApp();
    void initActiveCover();
    void initPlayer();
    void initTranslator();
private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    bb::cascades::Application * m_app;
    Player * m_player;
};

#endif
