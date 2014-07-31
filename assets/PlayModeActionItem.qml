import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    title: qsTr("Playlist Mode")
    imageSource: "asset:///images/actions/icon_playlist_mode.png"
    enabled: !app.player.currentSong.isHttpSong
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
                appendItem(qsTr("One Song in Loop"), true, mode == Playlist.SongCycle)
                appendItem(qsTr("Playlist Once"), true, mode == Playlist.PlaylistOnce)
                appendItem(qsTr("Playlist in Loop"), true, mode == Playlist.PlaylistCycle)
                appendItem(qsTr("Playlist in Random Order"), true, mode == Playlist.PlaylistRandomOnce)
                appendItem(qsTr("Playlist Repeating Random"), true, mode == Playlist.PlaylistRandomCycle)
                exec()
                if(result == SystemUiResult.ConfirmButtonSelection ){
                    var modes = [Playlist.SongCycle,
                                 Playlist.PlaylistOnce, 
                                 Playlist.PlaylistCycle,
                                 Playlist.PlaylistRandomOnce,
                                 Playlist.PlaylistRandomCycle];
                    app.player.playlist.mode = modes[selectedIndices[0]]
                }
            }
        }
    ]
}