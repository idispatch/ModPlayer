import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    title: qsTr("Create Playlist")
    imageSource: "asset:///images/actions/icon_append_playlist.png"
    onTriggered: {
        var name = playlistName.run()
        if(name.length < 1)
            return;
        app.catalog.createPlaylist(name)
        playlistCreated.body = qsTr("Playlist '%1' has been created").arg(name)
        playlistCreated.show()
    }
    attachedObjects: [
        SystemToast {
            id: playlistCreated
        },
        SystemPrompt {
            id: playlistName
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