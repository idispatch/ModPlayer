import bb.cascades 1.0
import player 1.0

ActionItem {
    title: {
        if(app.player.state == Player.Playing ||
           app.player.state == Player.Paused) {
            return "Stop"
        } else {
            return "Play"
        }
    }
    enabled: app.player.currentSong.songLoaded
    imageSource: {
        if(app.player.state == Player.Playing ||
           app.player.state == Player.Paused) {
            return "asset:///images/actions/icon_stop.png"
        } else {
            return "asset:///images/actions/icon_play.png"
        }
    }
    ActionBar.placement: ActionBarPlacement.OnBar
    onTriggered: {
        if(app.player.state == Player.Playing ||
           app.player.state == Player.Paused) {
            app.player.stop()
        } else {
            app.player.play(currentSong)
        }
    }
}