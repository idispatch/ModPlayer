import bb.cascades 1.0
import player 1.0

ActionItem {
    title: qsTr("Open Song from Device") + Retranslate.onLanguageChanged
    imageSource: app.isExtendedVersion ? "asset:///images/actions/icon_load_song.png" : "asset:///images/actions/icon_lock.png"
    enabled: app.isExtendedVersion
    onTriggered: {
        app.player.browseForLocalSong()
    }
}