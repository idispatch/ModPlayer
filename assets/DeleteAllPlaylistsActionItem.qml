import bb.cascades 1.3
import bb.system 1.2
import player 1.0

ActionItem {
    signal playlistsDeleted()
    title: qsTr("Delete All Playlists") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_delete.png"
    onTriggered: {
        confirmDeleteAllPlaylists.exec()
    }
    attachedObjects: [
        SystemDialog {
            id: confirmDeleteAllPlaylists
            title: qsTr("Confirm") + Retranslate.onLanguageChanged
            body: qsTr("All playlists will be deleted. Please confirm.") + Retranslate.onLanguageChanged
            onFinished: {
                if (result != SystemUiResult.ConfirmButtonSelection)
                    return;
                app.catalog.deleteAllPlaylists()
                playlistsDeleted()
                playlistsDeletedToast.show()
            }
        },
        SystemToast {
            id: playlistsDeletedToast
            body: qsTr("All playlists have been deleted") + Retranslate.onLanguageChanged
        }
    ]
}