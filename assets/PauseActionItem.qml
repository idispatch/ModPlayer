import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant navigationPane
    property variant playlistId: null
    property variant albumId: null
    title: {
        if(playlistId != null) {
            return qsTr("Play Playlist") + Retranslate.onLanguageChanged
        } else if(albumId != null) {
            return qsTr("Play Album") + Retranslate.onLanguageChanged
        } else {
            if(app.player.state == Player.Paused || app.player.state == Player.Stopped) {
                return qsTr("Resume") + Retranslate.onLanguageChanged
            } else {
                return qsTr("Pause") + Retranslate.onLanguageChanged
            }
        }
    }
    enabled: {
        return playlistId != null ||
               albumId != null ||
               ((app.player.currentSong.songLoaded || app.player.currentSong.isHttpSong) &&
                (app.player.state == Player.Paused || 
                 app.player.state == Player.Playing))
    }
    imageSource: {
        if(app.player.state == Player.Paused || app.player.state == Player.Stopped || playlistId != null || albumId != null) {
            return "asset:///images/actions/icon_play.png"
        } else {
            return "asset:///images/actions/icon_pause.png"
        }
    } 
    function enqueuAndPlay(songs) {
        if(songs.length > 0) {
            app.player.playlist.assign(songs)
            app.player.playPlaylist()
            var view = songPlayer.createObject()
            if(view){
                view.navigationPane = navigationPane
                navigationPane.push(view)
            }
        }
    }
    onTriggered: {
        if(playlistId != null) {
            enqueuAndPlay(app.catalog.getPlaylistSongs(playlistId))
        } else if(albumId != null) {
            enqueuAndPlay(app.catalog.getAlbumSongs(albumId))
        } else {
            if(app.player.state == Player.Paused) {
                app.player.resume()
            } else if(app.player.state == Player.Playing) {
                app.player.pause()
            }
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
}