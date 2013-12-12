import bb.cascades 1.0
import player 1.0

ActionItem {
    title: qsTr("Previous")
    enabled: app.player.currentSong.songLoaded
    imageSource: "asset:///images/actions/icon_previous.png"
    ActionBar.placement: ActionBarPlacement.OnBar
    onTriggered: {
        app.player.playPrevious()
    }
}