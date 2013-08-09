import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant currentSong
    
    title: "Remove from Favourites"
    imageSource: "asset:///images/icon_favorite_off.png"
    enabled: currentSong != null
    ActionBar.placement: ActionBarPlacement.InOverflow
    shortcuts: Shortcut {
        key: "u"
    } 
    onTriggered: {
        if (currentSong != null) {
            app.catalog.removeFavourite(currentSong)
            currentSong.myFavourite = 0
        }
    }
}