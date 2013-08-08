import bb.cascades 1.0

ActionItem {
    property variant currentSong
    
    title: "Add to Favourites"
    imageSource: "asset:///images/icon_favorite.png"
    enabled: currentSong != null
    ActionBar.placement: ActionBarPlacement.InOverflow
    shortcuts: Shortcut {
        key: "f"
    } 
    onTriggered: {
        if (currentSong != null) {
            app.catalog.addFavourite(currentSong)
            currentSong.myFavourite = 1
        }
    }
}