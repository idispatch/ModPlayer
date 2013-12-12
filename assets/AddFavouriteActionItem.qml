import bb.cascades 1.0

ActionItem {
    property variant currentSong
    
    title: qsTr("Add to Favourites")
    imageSource: "asset:///images/actions/icon_like.png"
    enabled: currentSong != null && currentSong.songLoaded && currentSong.id != 0
    shortcuts: Shortcut {
        key: "f"
    } 
    onTriggered: {
        if (currentSong != null) {
            app.catalog.addFavourite(currentSong)
        }
    }
}