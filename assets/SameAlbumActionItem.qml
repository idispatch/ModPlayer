import bb.cascades 1.0

ActionItem {
    property variant currentSong
    property variant navigationPane
    title: qsTr("Same Album Songs") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_albums.png"
    enabled: currentSong && currentSong.id != 0 && !currentSong.isTrackerSong && !currentSong.isHttpSong
    onTriggered: {
        var previousPage = navigationPane.top
        var view = songList.createObject()
        view.navigationPane = navigationPane
        navigationPane.push(view)
        navigationPane.remove(previousPage)
        view.loadSongsBySongAlbum(currentSong.id)
    }
    attachedObjects: [
        ComponentDefinition {
            id: songList
            source: "SongList.qml"
        }
    ]
}