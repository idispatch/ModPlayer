import bb.cascades 1.3
import player 1.0

ActionItem {
    title: qsTr("Open Song from Device")
    imageSource: "asset:///images/actions/icon_load_song.png"
    onTriggered: {
        app.player.browseForLocalSong()
    }
}