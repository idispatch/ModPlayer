import bb.cascades 1.3
import player 1.0

ActionItem {
    title: qsTr("Previous") + Retranslate.onLanguageChanged
    enabled: app.player.playlist.previousAvailable
    imageSource: "asset:///images/actions/icon_previous.png"
    onTriggered: {
        app.player.playPrevious()
    }
}