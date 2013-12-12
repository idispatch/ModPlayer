import bb.cascades 1.0
import player 1.0

ActionItem {
    title: {
        if(app.player.state == Player.Paused) {
            return qsTr("Resume")
        } else if(app.player.state == Player.Playing) {
        	return qsTr("Pause")
        } else {
            return qsTr("Play")
        }
    }
    enabled: {
        return app.player.currentSong.songLoaded &&
                (app.player.state == Player.Paused || 
                 app.player.state == Player.Playing);
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
}