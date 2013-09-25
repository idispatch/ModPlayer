import bb.cascades 1.0
import player 1.0

ActionItem {
    title: {
        if(app.player.mode == Player.PlaySongOnce) {
            return qsTr("Play Once")
        } else if(app.player.mode == Player.RepeatSong) {
            return qsTr("Repeat Song")
        } else if(app.player.mode == Player.PlayPlaylistOnce) {
            return qsTr("Playlist Once")
        } else if(app.player.mode == Player.RepeatPlaylist) {
            return qsTr("Cycle Playlist")
        } else if(app.player.mode == Player.RandomSongPlaylist) {
            return qsTr("Random Playlist")
        }
        return ""
    }
    enabled: app.player.currentSong.songLoaded
    imageSource: {
        if(app.player.mode == Player.PlaySongOnce) {
            return "asset:///images/actions/icon_playonce.png"
        } else if(app.player.mode == Player.RepeatSong) {
            return "asset:///images/actions/icon_repeatsong.png"
        } else if(app.player.mode == Player.PlayPlaylistOnce) {
            return "asset:///images/actions/icon_repeatsong.png"
        } else if(app.player.mode == Player.RepeatPlaylist) {
            return "asset:///images/actions/icon_repeatsong.png"
        } else if(app.player.mode == Player.RandomSongPlaylist) {
            return "asset:///images/actions/icon_repeatsong.png"
        }
        return "asset:///images/actions/icon_playonce.png"
    }
    onTriggered: {
        if(app.player.mode == Player.PlaySongOnce) {
            app.player.mode = Player.RepeatSong
        } else if(app.player.mode == Player.RepeatSong) {
            app.player.mode = Player.PlaySongOnce
        } else {
            app.player.mode = Player.PlaySongOnce
        }
    }
}