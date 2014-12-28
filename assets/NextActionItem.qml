import bb.cascades 1.0
import player 1.0

ActionItem {
    title: qsTr("Next") + Retranslate.onLanguageChanged
    enabled: app.player.playlist.nextAvailable
    imageSource: "asset:///images/actions/icon_next.png"
    onTriggered: {
        app.player.playNext()
    }
}