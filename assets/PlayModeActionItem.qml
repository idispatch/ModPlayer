import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    title: qsTr("Playlist Mode")
    enabled: app.player.currentSong.songLoaded
    imageSource: "asset:///images/actions/icon_playlist_mode.png"
    onTriggered: selectPlayMode.run()
    attachedObjects: [
        SystemListDialog {
            id: selectPlayMode
            title: qsTr("Select Play Mode")
            selectionMode: ListSelectionMode.Single
            modality: SystemUiModality.Application
            includeRememberMe: false
            emoticonsEnabled: false
            defaultButton: SystemUiButton.confirmButton
            dismissAutomatically: true
            function run() {
                var mode = app.player.playlist.mode
                clearList()
                appendItem(qsTr("Song Once"), true, mode == Playlist.SongOnce)
                appendItem(qsTr("Song Repeat"), true, mode == Playlist.SongCycle)
                appendItem(qsTr("Playlist Once"), true, mode == Playlist.PlaylistOnce)
                appendItem(qsTr("Playlist Repeat"), true, mode == Playlist.PlaylistCycle)
                appendItem(qsTr("Shuffle Playlist"), true, mode == Playlist.PlaylistRandomOnce)
                appendItem(qsTr("Repeat Shuffle Playlist"), true, mode == Playlist.PlaylistRandomCycle)
                exec()
                if(result == SystemUiResult.ConfirmButtonSelection ){
                    var modes = [Playlist.SongOnce, 
                                 Playlist.SongCycle,
                                 Playlist.PlaylistOnce, 
                                 Playlist.PlaylistCycle,
                                 Playlist.PlaylistRandomOnce,
                                 Playlist.PlaylistRandomCycle];
                    console.log(selectedIndices[0])
                    console.log(modes[selectedIndices[0]])
                    app.player.playlist.mode = modes[selectedIndices[0]]
                    console.log(app.player.playlist.mode)
                }
            }
        }
    ]
}