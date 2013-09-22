import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant currentSong
    
    title: qsTr("Remove from Favourites")
    imageSource: "asset:///images/actions/icon_unlike.png"
    enabled: currentSong != null && currentSong.songLoaded && currentSong.id != 0
    ActionBar.placement: ActionBarPlacement.InOverflow
    shortcuts: Shortcut {
        key: "u"
    } 
    onTriggered: {
        if (currentSong != null) {
            app.catalog.removeFavourite(currentSong)
        }
    }
}