import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    property variant songList
    title: qsTr("Play All") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_set_playlist.png"
    enabled: songList!=null && songList.size() > 0
    onTriggered: {
        if (songList != null) {
            app.player.stop()
            var songCount = songList.size()
            var songIds = []
            for(var i = 0; i < songCount; i++) {
                songIds.push(songList.value(i).id)
            }
            app.player.playlist.assign(songIds)
            app.player.playPlaylist()
        }
    }
}