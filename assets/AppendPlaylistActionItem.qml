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
                appendItem(qsTr("One"), true, false)
                appendItem(qsTr("Two"), true, false)
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
                    playlistCreatedToast.body = qsTr("Playlist '%1' has been created").arg(playlistName)
                    playlistCreatedToast.show()
                } else if(item >= 3) {
                    item -= 3;
                    console.log(item)
                }
            }
        },
        SystemToast {
            id: playlistCreatedToast
        },
        PlaylistNameEntryPrompt {
            id: playlistNameEntryPrompt
        }
    ]
}