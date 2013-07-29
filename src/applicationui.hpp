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

class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);

    Q_PROPERTY(Player* player READ player NOTIFY playerChanged FINAL)

    Player * player() const;
Q_SIGNALS:
    void playerChanged();
private slots:
    void onSystemLanguageChanged();
private:
    void initApp();
    void initTranslator();
private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;
    bb::cascades::Application * m_app;
    Player * m_player;
};

#endif
