import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    title: qsTr("Import Songs")
    imageSource: "asset:///images/actions/icon_import.png"
    onTriggered: {
        confirmImport.show()
    }
    attachedObjects: [
        SystemDialog {
            id: confirmImport
            title: qsTr("Confirm")
            body: qsTr("Would you like to import local songs and playlists?")
            onFinished: {
                if (result == SystemUiResult.ConfirmButtonSelection) {
                    app.player.importSongs()
                }
            }
        }
    ] 
}
