import bb.cascades 1.0
import player 1.0

ActionItem {
    title: {
        if(app.player.currentSong.songLoaded) {
            if(app.player.state == Player.Playing) {
                return qsTr("Stop") + Retranslate.onLanguageChanged
            } else if(app.player.state == Player.Paused) {
                return qsTr("Stop") + Retranslate.onLanguageChanged
            } else {
                return qsTr("Play") + Retranslate.onLanguageChanged
            }
        } else {
            if(app.player.playlist.count > 0) {
                return qsTr("Play") + Retranslate.onLanguageChanged
            } else {
                return qsTr("Stop") + Retranslate.onLanguageChanged
            }
        }
    }
    enabled: app.player.currentSong.songLoaded || app.player.playlist.remaining > 0 || app.player.currentSong.isHttpSong
    imageSource: {
        if(app.player.currentSong.songLoaded) {
            if(app.player.state == Player.Playing) {
                return "asset:///images/actions/icon_stop.png"
            } else if(app.player.state == Player.Paused) {
                return "asset:///images/actions/icon_stop.png"
            } else {
                return "asset:///images/actions/icon_play.png"
            }
        } else {
            if(app.player.playlist.count > 0) {
                return "asset:///images/actions/icon_play.png"
            } else {
                return "asset:///images/actions/icon_stop.png"
            }
        }
    }
    onTriggered: {
        if(app.player.state == Player.Playing || app.player.state == Player.Paused) {
            app.player.stop()
        } else {
            if(app.player.currentSong && app.player.currentSong.songLoaded) {
                app.player.play(app.player.currentSong)
            } else {
                app.player.playPlaylist()
            }
        }
    }
}