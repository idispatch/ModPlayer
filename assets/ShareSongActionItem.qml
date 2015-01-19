import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant currentSong
    title: qsTr("Share Song") + Retranslate.onLanguageChanged
    imageSource: app.isExtendedVersion ? "asset:///images/actions/icon_share.png" : "asset:///images/actions/icon_lock.png"
    enabled: currentSong != null && currentSong.songLoaded && !currentSong.isHttpSong
    onTriggered: {
        if(app.isExtendedVersion) {
            if (currentSong != null && currentSong.songLoaded && !currentSong.isHttpSong) {
                app.shareSong(currentSong.absoluteFileName)
            }
        } else {
            app.pleaseBuy()
        }
    }
}