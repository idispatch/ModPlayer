import bb.cascades 1.3
import bb.system 1.2
import player 1.0

ActionItem {
    signal playlistCreated()
    title: qsTr("Create Playlist") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_append_playlist.png"
    onTriggered: {
        var playlistName = playlistNamePrompt.run()
        if(playlistName.length < 1)
            return;
        app.catalog.createPlaylist(playlistName)
        playlistCreatedToast.body = qsTr("Playlist '%1' has been created").arg(playlistName) + Retranslate.onLanguageChanged
        playlistCreatedToast.show()
        playlistCreated()
    }
    attachedObjects: [
        SystemToast {
            id: playlistCreatedToast
        },
        PlaylistNameEntryPrompt {
            id: playlistNamePrompt
        }
    ]
}