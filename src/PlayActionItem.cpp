#include "PlayActionItem.hpp"
#include "ApplicationUI.hpp"
#include "Player.hpp"
#include "Playlist.hpp"
#include "SongExtendedInfo.hpp"
#include "SongModule.hpp"

PlayActionItem::PlayActionItem(QObject * parent) {
    setParent(parent);

    int rc;
    Q_UNUSED(rc);

    rc = QObject::connect(player(), SIGNAL(stateChanged()),
                          this, SLOT(update()));
    Q_ASSERT(rc);

    rc = QObject::connect(currentSong(), SIGNAL(songLoadedChanged()),
                          this, SLOT(update()));
    Q_ASSERT(rc);

    rc = QObject::connect(currentSong(), SIGNAL(isHttpSongChanged()),
                          this, SLOT(update()));
    Q_ASSERT(rc);

    rc = QObject::connect(playlist(), SIGNAL(remainingCountChanged()),
                          this, SLOT(update()));
    Q_ASSERT(rc);

    rc = QObject::connect(this, SIGNAL(triggered()),
                          this, SLOT(onActionTriggered()));
    Q_ASSERT(rc);

    rc = QObject::connect(this, SIGNAL(creationCompleted()),
                          this, SLOT(update()));
    Q_ASSERT(rc);
}

Player * PlayActionItem::player() {
    return ApplicationUI::instance().player();
}

Playlist * PlayActionItem::playlist() {
    return player()->playlist();
}

SongModule * PlayActionItem::currentSong() {
    return player()->currentSong();
}

void PlayActionItem::update() {
    QString text = title();
    QUrl imageUrl = imageSource();
    bool enabled = isEnabled();

    bool showPlay = false;

    switch(player()->state()) {
    case Player::Playing:
    case Player::Paused:
        showPlay = false;
        enabled = true;
        break;
    case Player::Stopped:
        if(currentSong()->songLoaded() ||
           currentSong()->isHttpSong() ||
           playlist()->remainingCount() > 0) {
            showPlay = true;
            enabled = true;
        } else {
            showPlay = false;
            enabled = false;
        }
        break;
    case Player::Resolving:
    case Player::Downloading:
    case Player::Preparing:
    default:
        showPlay = false;
        enabled = false;
        break;
    }

    if(showPlay) {
        imageUrl = QString("asset:///images/actions/icon_play.png");
        text = tr("Play");
    } else {
        imageUrl = QString("asset:///images/actions/icon_stop.png");
        text = tr("Stop");
    }

    if(imageUrl != imageSource()) {
        setImageSource(imageUrl);
    }
    if(text != title()) {
        setTitle(text);
    }
    if(enabled != isEnabled()) {
        setEnabled(enabled);
    }
}

void PlayActionItem::onActionTriggered() {
    switch(player()->state()) {
    case Player::Playing:
    case Player::Paused:
        player()->stop();
        break;
    case Player::Stopped:
        if(currentSong()->songLoaded() ||
           currentSong()->isHttpSong()) {
            QVariant song = QVariant::fromValue(static_cast<QObject*>(currentSong()));
            player()->play(song);
        } else {
            if(playlist()->remainingCount() > 0) {
                player()->playPlaylist();
            }
        }
        break;
    case Player::Resolving:
    case Player::Downloading:
    case Player::Preparing:
    default:
        break;
    }
}
