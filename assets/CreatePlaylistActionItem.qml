import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    signal playlistCreated()
    title: qsTr("Create Playlist")
    imageSource: "asset:///images/actions/icon_append_playlist.png"
    onTriggered: {
        var playlistName = playlistNamePrompt.run()
        if(playlistName.length < 1)
            return;
        app.catalog.createPlaylist(playlistName)
        playlistCreatedToast.body = qsTr("Playlist '%1' has been created").arg(playlistName)
        playlistCreatedToast.show()
        playlistCreated()
    }
    attachedObjects: [
        SystemToast {
            id: playlistCreatedToast
        },
        SystemPrompt {
            id: playlistNamePrompt
            title: qsTr("Enter Playlist Name")
            modality: SystemUiModality.Application
            dismissAutomatically: true
            includeRememberMe: false
            inputField.emptyText: qsTr("Playlist Name")
            inputField.defaultText: "Playlist"
            inputField.inputMode: SystemUiInputMode.Default
            inputField.maximumLength: 20
            function run() {
                exec()
                if(result != SystemUiResult.ConfirmButtonSelection)
                    return ""
                return inputFieldTextEntry()
            }
        }
    ]
}