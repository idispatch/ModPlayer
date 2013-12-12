import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    signal playlistsDeleted()
    title: qsTr("Delete All Playlists")
    imageSource: "asset:///images/actions/icon_delete.png"
    onTriggered: {
        confirmDeleteAllPlaylists.exec()
    }
    attachedObjects: [
        SystemDialog {
            id: confirmDeleteAllPlaylists
            title: qsTr("Confirm")
            body: qsTr("All playlists will be deleted. Please confirm.")
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
            body: qsTr("All playlists have been deleted")
        }
    ]
}