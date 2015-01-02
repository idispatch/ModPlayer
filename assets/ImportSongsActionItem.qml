import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    title: qsTr("Import Songs") + Retranslate.onLanguageChanged
    imageSource: app.isExtendedVersion ? "asset:///images/actions/icon_import.png" : "asset:///images/actions/icon_lock.png"
    enabled: app.isExtendedVersion
    onTriggered: {
        confirmImport.show()
    }
    attachedObjects: [
        SystemDialog {
            id: confirmImport
            title: qsTr("Confirm") + Retranslate.onLanguageChanged
            body: qsTr("Would you like to import local songs and playlists?") + Retranslate.onLanguageChanged
            onFinished: {
                if (result == SystemUiResult.ConfirmButtonSelection) {
                    app.player.importSongs()
                }
            }
        }
    ] 
}
