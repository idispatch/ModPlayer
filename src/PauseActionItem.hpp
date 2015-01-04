#ifndef PAUSEACTIONITEM_HPP
#define PAUSEACTIONITEM_HPP

#include <QObject>
#include <QMetaType>
#include <bb/cascades/ActionItem>

class Player;
class Playlist;
class SongModule;

class PauseActionItem : public bb::cascades::ActionItem {
    Q_OBJECT
    Q_DISABLE_COPY(PauseActionItem)
public:
    PauseActionItem(QObject * parent = 0);
private slots:
    void update();
    void onActionTriggered();
private:
    Player * player();
    Playlist * playlist();
    SongModule * currentSong();
};

Q_DECLARE_METATYPE(PauseActionItem*);

#endif
