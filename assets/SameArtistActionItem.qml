import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant currentSong
    property variant navigationPane
    title: qsTr("Songs by Artist") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_artist.png"
    enabled: currentSong && currentSong.artistId != 0 && !currentSong.isHttpSong 
    onTriggered: {
        var artistId = currentSong.artistId
        var artistName = currentSong.artist
        var previousPage = navigationPane.top
        var view = songList.createObject()
        if(view) {
            view.navigationPane = navigationPane
            navigationPane.push(view)
            navigationPane.remove(previousPage)
            view.loadSongsByArtist(artistId, artistName)
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: songList
            source: "SongList.qml"
        }
    ]
}