import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant currentSong
    title: qsTr("Share Song") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_share.png"
    enabled: currentSong != null && currentSong.songLoaded && currentSong.id != 0 && !currentSong.isHttpSong
    onTriggered: {
        if (currentSong != null && !currentSong.isHttpSong) {
            app.shareSong(currentSong.absoluteFileName)
        }
    }
}