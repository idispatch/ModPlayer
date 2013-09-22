import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant navigationPane
    title: qsTr("Open song from device")
    imageSource: "asset:///images/actions/icon_open_song.png"
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered: {
        app.player.browseForLocalSong()
    }
}