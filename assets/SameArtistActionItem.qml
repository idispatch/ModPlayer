import bb.cascades 1.0

ActionItem {
    property variant currentSong
    property variant navigationPane
    title: qsTr("Songs by Artist")
    imageSource: "asset:///images/actions/icon_artist.png"
    enabled: currentSong && currentSong.artistId != 0 
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered: {
        var artistId = currentSong.artistId
        var artistName = currentSong.artist
        var previousPage = navigationPane.top
        var view = songList.createObject()
        view.navigationPane = navigationPane
        navigationPane.push(view)
        navigationPane.remove(previousPage)
        view.loadSongsByArtist(artistId, artistName)
    }
    attachedObjects: [
        ComponentDefinition {
            id: songList
            source: "SongList.qml"
        }
    ]
}