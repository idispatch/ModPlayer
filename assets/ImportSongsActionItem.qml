import bb.cascades 1.3
import bb.system 1.2
import player 1.0

ActionItem {
    signal completed
    title: qsTr("Import Songs") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_import.png"
    onTriggered: {
        app.player.importSongs()
        completed()
    }
}
