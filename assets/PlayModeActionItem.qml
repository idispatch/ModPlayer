import bb.cascades 1.0
import player 1.0

ActionItem {
    title: {
        if(app.player.playlist.mode == Playlist.SongOnce) {
            return qsTr("Song Once")
        } else if(app.player.playlist.mode == Playlist.SongCycle) {
            return qsTr("Song Loop")
        } else if(app.player.playlist.mode == Playlist.PlaylistOnce) {
            return qsTr("List Once")
        } else if(app.player.playlist.mode == Playlist.PlaylistCycle) {
            return qsTr("List Loop")
        } else if(app.player.playlist.mode == Playlist.PlaylistRandomOnce) {
            return qsTr("List Rand")
        } else if(app.player.playlist.mode == Playlist.PlaylistRandomCycle) {
            return qsTr("Rand Loop")
        }
        return ""
    }
    enabled: app.player.currentSong.songLoaded
    imageSource: {
        if(app.player.playlist.mode == Playlist.SongOnce) {
            return "asset:///images/actions/icon_playonce.png"
        } else if(app.player.playlist.mode == Playlist.SongCycle) {
            return "asset:///images/actions/icon_repeatsong.png"
        } else if(app.player.playlist.mode == Playlist.PlaylistOnce) {
            return "asset:///images/actions/icon_repeatsong.png"
        } else if(app.player.playlist.mode == Playlist.PlaylistCycle) {
            return "asset:///images/actions/icon_repeatsong.png"
        } else if(app.player.playlist.mode == Playlist.PlaylistRandomOnce) {
            return "asset:///images/actions/icon_repeatsong.png"
        } else if(app.player.playlist.mode == Playlist.PlaylistRandomCycle) {
            return "asset:///images/actions/icon_repeatsong.png"
        }
        return "asset:///images/actions/icon_playonce.png"
    }
    onTriggered: {
        if(app.player.playlist.mode == Playlist.SongOnce) {
            app.player.playlist.mode = Playlist.SongCycle
        } else if(app.player.playlist.mode == Playlist.SongCycle) {
            app.player.playlist.mode = Playlist.PlaylistOnce
        } else if(app.player.playlist.mode == Playlist.PlaylistOnce) {
            app.player.playlist.mode = Playlist.PlaylistCycle
        } else if(app.player.playlist.mode == Playlist.PlaylistCycle) {
            app.player.playlist.mode = Playlist.PlaylistRandomOnce
        } else if(app.player.playlist.mode == Playlist.PlaylistRandomOnce) {
            app.player.playlist.mode = Playlist.PlaylistRandomCycle
        } else if(app.player.playlist.mode == Playlist.PlaylistRandomCycle) {
            app.player.playlist.mode = Playlist.SongCycle
        } else {
            app.player.playlist.mode = Playlist.SongCycle
        }
    }
}