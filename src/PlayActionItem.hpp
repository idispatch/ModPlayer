#ifndef PLAYACTIONITEM_HPP
#define PLAYACTIONITEM_HPP

#include <QObject>
#include <QMetaType>
#include <bb/cascades/ActionItem>

class Player;
class Playlist;
class SongModule;

class PlayActionItem : public bb::cascades::ActionItem {
    Q_OBJECT
    Q_DISABLE_COPY(PlayActionItem)
public:
    PlayActionItem(QObject * parent = 0);
private slots:
    void update();
    void onActionTriggered();
private:
    Player * player();
    Playlist * playlist();
    SongModule * currentSong();
};

Q_DECLARE_METATYPE(PlayActionItem*);

#endif
