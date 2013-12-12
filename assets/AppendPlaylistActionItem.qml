import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    property variant currentSong
    title: qsTr("Append to Playlist")
    imageSource: "asset:///images/actions/icon_append_playlist.png"
    enabled: currentSong != null && currentSong.id != 0
    onTriggered: {
        if (currentSong != null) {
            playlistSelection.run()
        }
    }
    attachedObjects: [
        SystemListDialog {
            id: playlistSelection
            title: qsTr("Select Playlist")
            selectionMode: ListSelectionMode.Single
            modality: SystemUiModality.Application
            includeRememberMe: false
            emoticonsEnabled: false
            defaultButton: SystemUiButton.confirmButton
            dismissAutomatically: true
            function run() {
                clearList()
                appendItem(qsTr("Current Playlist"), true, true)
                appendItem(qsTr("New Playlist..."), true, false)
                appendHeader(qsTr("Existing Playlists"))
                
                var model = app.catalog.findPlaylists()
                var playlists = {}
                var counter = 3
                for(var i = model.first(); 
                    i.length > 0; 
                    i = model.after(i)) {
                    var value = model.data(i)
                    playlists[counter++] = value
                    appendItem(value.name, true, false)
                }
                exec()
                
                if(result != SystemUiResult.ConfirmButtonSelection )
                    return
                if(selectedIndices.length < 1)
                    return
                var item = selectedIndices[0]
                if(item == 0) {
                    app.player.playlist.add(currentSong.id)
                } else if(item == 1) {
                    var playlistName = playlistNameEntryPrompt.run()
                    if(playlistName.length < 1)
                        return;
                    app.catalog.createPlaylist(playlistName)
                    notificationToast.body = qsTr("Playlist '%1' has been created").arg(playlistName)
                    notificationToast.show()
                } else if(item >= 3) {
                    app.catalog.appendToPlaylist(playlists[item].id, currentSong.id)
                    notificationToast.body = qsTr("Song '%1' was added to playlist '%2'").arg(currentSong.title).arg(playlists[item].name)
                    notificationToast.show()
                }
            }
        },
        SystemToast {
            id: notificationToast
        },
        PlaylistNameEntryPrompt {
            id: playlistNameEntryPrompt
        }
    ]
}