import bb.cascades 1.0
import player 1.0

ActionItem {
    title: qsTr("Next")
    enabled: app.player.currentSong.songLoaded
    imageSource: "asset:///images/actions/icon_next.png"
    ActionBar.placement: ActionBarPlacement.OnBar
    onTriggered: {
        app.player.playNext()
    }
}