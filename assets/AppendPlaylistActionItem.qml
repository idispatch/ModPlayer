import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    property variant songList
    property variant currentSong
    property variant rootObject
    property string mode: ""
    title: qsTr("Append to Playlist") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_append_playlist.png"
    enabled: {
        if(mode != 'playlist') {
            if(currentSong != null) {
                return currentSong.id != 0
            } else if (songList!=null) {
                return songList.size() > 0
            }
        }
        return false
    }
    onTriggered: {
        if (currentSong != null || songList != null) {
            playlistSelection.run()
        }
    }
    attachedObjects: [
        SystemListDialog {
            id: playlistSelection
            title: qsTr("Select Playlist") + Retranslate.onLanguageChanged
            selectionMode: ListSelectionMode.Single
            modality: SystemUiModality.Application
            includeRememberMe: false
            emoticonsEnabled: false
            defaultButton: SystemUiButton.confirmButton
            dismissAutomatically: true
            function addToPlaylist(playlistId, playlistName) {
                var root = null
                if(rootObject != null) {
                    root = rootObject 
                } else {
                    root = app
                }
                if(currentSong != null) {
                    root.catalog.appendToPlaylist(playlistId, currentSong.id)
                    notificationToast.body = qsTr("Song '%1' was added to playlist '%2'").arg(currentSong.title).arg(playlistName) + Retranslate.onLanguageChanged
                    notificationToast.show()
                } else if(songList != null) {
                    var songCount = songList.size()
                    if(songCount > 0) {
                        if(songCount > 1000) {
                            songCount = 1000
                        }
                        var songs = []
                        for(var i = 0; i < songCount; i++) {
                            songs.push(songList.value(i).id)
                        }
                        root.catalog.appendSongsToPlaylist(playlistId, songs)
                        notificationToast.body = qsTr("%1 songs added to playlist '%2'").arg(songCount).arg(playlistName) + Retranslate.onLanguageChanged
                        notificationToast.show()
                    }
                }
            }
            function run() {
                if(currentSong == null && songList == null)
                    return
                var root = null
                if(rootObject != null) {
                    root = rootObject 
                } else {
                    root = app
                }
                clearList()
                appendItem(qsTr("Current Playlist (%1 songs)").arg(root.player.playlist.count) + Retranslate.onLanguageChanged, true, true)
                appendItem(qsTr("New Playlist...") + Retranslate.onLanguageChanged, true, false)
                var model = root.catalog.findPlaylists("")
                var playlists = {}
                var counter = 3
                var headerAdded = false
                for(var i = model.first(); 
                    i.length > 0; 
                    i = model.after(i)) {
                    var value = model.data(i)
                    playlists[counter++] = value
                    if(!headerAdded) {
                        appendHeader(qsTr("Existing Playlists") + Retranslate.onLanguageChanged)
                        headerAdded = true
                    }
                    var playlistTitle = qsTr("%1 (%2 songs)").arg(value.name).arg(value.count) + Retranslate.onLanguageChanged 
                    appendItem(playlistTitle, true, false)
                }
                model.destroy()
                exec()
                if(result == SystemUiResult.ConfirmButtonSelection && 
                   selectedIndices.length == 1) {
                    var item = selectedIndices[0]
                    if(item == 0) {
                        // Add to current playlist
                        if(currentSong != null) {
                            root.player.playlist.add(currentSong.id)
                        } else if(songList != null) {
                            var songCount = songList.size()
                            if(songCount > 0) {
                                for(var i = 0; i < songCount; i++) {
                                    var songId = songList.value(i).id
                                    root.player.playlist.add(songId)
                                }
                            }
                        }
                    } else if(item == 1) {
                        // Add to new playlist
                        var playlistName = playlistNameEntryPrompt.run()
                        if(playlistName.length < 1)
                            return;
                        var playlistId = root.catalog.createPlaylist(playlistName)
                        addToPlaylist(playlistId, playlistName)
                    } else if(item >= 3) {
                        // Add to existing playlist
                        var playlistId = playlists[item].id
                        var playlistName = playlists[item].name
                        addToPlaylist(playlistId, playlistName)
                    }
                }
                playlists = undefined
            }
        },
        SystemToast {
            id: notificationToast
        },
        ProgressToast {
            id: progress
        },
        PlaylistNameEntryPrompt {
            id: playlistNameEntryPrompt
        }
    ]
}