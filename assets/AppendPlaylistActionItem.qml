import bb.cascades 1.0

ActionItem {
    property variant currentSong
    title: qsTr("Append to Playlist")
    imageSource: "asset:///images/actions/icon_append_playlist.png"
    enabled: currentSong != null && currentSong.id != 0
    onTriggered: {
        if (currentSong != null) {
            app.player.playlist.add(currentSong.id)
        }
    }
}