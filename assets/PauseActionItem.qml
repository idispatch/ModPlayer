import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant navigationPane
    title: {
        if(app.player.state == Player.Paused || app.player.state == Player.Stopped) {
            return qsTr("Resume") + Retranslate.onLanguageChanged
        } else {
            return qsTr("Pause") + Retranslate.onLanguageChanged
        }
    }
    enabled: {
        return (app.player.currentSong.songLoaded || app.player.currentSong.isHttpSong) &&
               (app.player.state == Player.Paused || app.player.state == Player.Playing)
    }
    imageSource: {
        if(app.player.state == Player.Paused || app.player.state == Player.Stopped) {
            return "asset:///images/actions/icon_play.png"
        } else {
            return "asset:///images/actions/icon_pause.png"
        }
    } 
    onTriggered: {
        if(app.player.state == Player.Paused) {
            app.player.resume()
        } else if(app.player.state == Player.Playing) {
            app.player.pause()
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
}