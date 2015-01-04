#include "PauseActionItem.hpp"
#include "ApplicationUI.hpp"
#include "Player.hpp"
#include "Playlist.hpp"
#include "SongExtendedInfo.hpp"
#include "SongModule.hpp"

PauseActionItem::PauseActionItem(QObject * parent) {
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

Player * PauseActionItem::player() {
    return ApplicationUI::instance().player();
}

Playlist * PauseActionItem::playlist() {
    return player()->playlist();
}

SongModule * PauseActionItem::currentSong() {
    return player()->currentSong();
}

void PauseActionItem::update() {
    QString text = title();
    QUrl imageUrl = imageSource();
    bool enabled = isEnabled();
    bool showPause = true;

    switch(player()->state()) {
    case Player::Paused:
        enabled = currentSong()->songLoaded() || currentSong()->isHttpSong();
        showPause = false;
        break;
    case Player::Playing:
        enabled = true;
        showPause = true;
        break;
    case Player::Stopped:
    case Player::Resolving:
    case Player::Downloading:
    case Player::Preparing:
    default:
        enabled = false;
        showPause = true;
        break;
    }

    if(showPause) {
        imageUrl = QString("asset:///images/actions/icon_pause.png");
        text = tr("Pause");
    } else {
        imageUrl = QString("asset:///images/actions/icon_play.png");
        text = tr("Resume");
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

void PauseActionItem::onActionTriggered() {
    switch(player()->state()) {
    case Player::Paused:
        player()->resume();
        break;
    case Player::Playing:
        player()->pause();
        break;
    case Player::Resolving:
    case Player::Downloading:
    case Player::Preparing:
    default:
        break;
    }
}
