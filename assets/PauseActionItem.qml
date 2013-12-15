import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant playlistId: none
    title: {
        if(playlistId != null) {
            return qsTr("Play Playlist")
        } else {
            if(app.player.state == Player.Playing) {
                return qsTr("Pause")
            } else {
                return qsTr("Resume")
            }
        }
    }
    enabled: {
        return playlistId != null ||
                (app.player.currentSong.songLoaded &&
                (app.player.state == Player.Paused || 
                 app.player.state == Player.Playing))
    }
    imageSource: {
        if(app.player.state == Player.Paused || app.player.state == Player.Stopped || playlistId != null) {
            return "asset:///images/actions/icon_play.png"
        } else {
            return "asset:///images/actions/icon_pause.png"
        }
    } 
    onTriggered: {
        if(playlistId != null) {
            var songs = app.catalog.getPlaylistSongs(playlistId)
            if(songs.length > 0) {
                app.player.playlist.clear()
                for(var i = 0; i < songs.length; ++i) {
                    app.player.playlist.add(songs[i])
                }
                app.player.playPlaylist()
                var view = songPlayer.createObject()
                view.navigationPane = navigationPane
                navigationPane.push(view)
            }
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