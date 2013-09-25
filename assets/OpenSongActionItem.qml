import bb.cascades 1.0
import player 1.0

ActionItem {
    title: qsTr("Open Song from Device")
    imageSource: "asset:///images/actions/icon_open_song.png"
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered: {
        app.player.browseForLocalSong()
    }
}