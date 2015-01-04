import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant currentSong
    
    title: qsTr("Add to Favourites") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_like.png"
    enabled: currentSong != null && currentSong.songLoaded && currentSong.id != 0 && !currentSong.isHttpSong
    shortcuts: Shortcut {
        key: "f"
    } 
    onTriggered: {
        if (currentSong != null && !currentSong.isHttpSong) {
            app.catalog.addFavourite(currentSong)
        }
    }
}