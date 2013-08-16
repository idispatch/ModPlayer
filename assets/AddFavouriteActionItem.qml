import bb.cascades 1.0

ActionItem {
    property variant currentSong
    
    title: "Add to Favourites"
    imageSource: "asset:///images/actions/icon_like.png"
    enabled: currentSong != null
    ActionBar.placement: ActionBarPlacement.InOverflow
    shortcuts: Shortcut {
        key: "f"
    } 
    onTriggered: {
        if (currentSong != null) {
            app.catalog.addFavourite(currentSong)
        }
    }
}