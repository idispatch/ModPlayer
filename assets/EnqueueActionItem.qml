import bb.cascades 1.0
import bb.system 1.0
import player 1.0

ActionItem {
    property variant songList
    title: qsTr("Enqueue") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_append_playlist.png"
    enabled: songList!=null && songList.size() > 0
    onTriggered: {
        if (songList != null) {
            var songCount = songList.size()
            for(var i = 0; i < songCount; i++) {
                app.player.playlist.add(songList.value(i).id)
            }
        }
    }
}