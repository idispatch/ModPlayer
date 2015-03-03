import bb.cascades 1.3
import bb.system 1.2
import player 1.0

ActionItem {
    title: qsTr("Playlist Mode") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_playlist_mode.png"
    enabled: !app.player.currentSong.isHttpSong
    onTriggered: selectPlayMode.run()
    attachedObjects: [
        SystemListDialog {
            id: selectPlayMode
            title: qsTr("Select Play Mode") + Retranslate.onLanguageChanged
            selectionMode: ListSelectionMode.Single
            modality: SystemUiModality.Application
            includeRememberMe: false
            emoticonsEnabled: false
            defaultButton: SystemUiButton.confirmButton
            dismissAutomatically: true
            function run() {
                var mode = app.player.playlist.mode
                clearList()
                appendItem(qsTr("One Song in Loop") + Retranslate.onLanguageChanged, true, mode == Playlist.SongCycle)
                appendItem(qsTr("Playlist Once") + Retranslate.onLanguageChanged, true, mode == Playlist.PlaylistOnce)
                appendItem(qsTr("Playlist in Loop") + Retranslate.onLanguageChanged, true, mode == Playlist.PlaylistCycle)
                appendItem(qsTr("Playlist in Random Order") + Retranslate.onLanguageChanged, true, mode == Playlist.PlaylistRandomOnce)
                appendItem(qsTr("Playlist Repeating Random") + Retranslate.onLanguageChanged, true, mode == Playlist.PlaylistRandomCycle)
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