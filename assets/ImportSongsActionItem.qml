import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    title: qsTr("Import Songs") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_import.png"
    onTriggered: {
        app.player.importSongs()
    }
}
