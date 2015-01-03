import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    title: qsTr("Import Songs") + Retranslate.onLanguageChanged
    imageSource: app.isExtendedVersion ? "asset:///images/actions/icon_import.png" : "asset:///images/actions/icon_lock.png"
    onTriggered: {
        if(!app.isExtendedVersion) {
            pleaseBuy.exec()
        } else {
            app.player.importSongs()
        }
    }
    attachedObjects: [
        PleaseBuy {
            id: pleaseBuy
        }
    ] 
}
