import bb.cascades 1.3
import bb.system 1.2
import player 1.0

ActionItem {
    property variant songList
    property bool checkExtendedVersion: false
    signal playbackStarted
    title: qsTr("Play All") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_set_playlist.png"
    enabled: songList && songList.size() > 0
    onTriggered: {
        if(songList) {
            if(checkExtendedVersion && !app.isExtendedVersion) {
                app.pleaseBuy()
            } else {
                var songCount = songList.size()
                var songItems = []
                for(var i = 0; i < songCount; i++) {
                    var songEntry = songList.value(i)
                    if(typeof(songEntry) == 'object') {
                        songItems.push(songEntry.id)
                    } else if(typeof(songEntry) == 'number') {
                        songItems.push(songEntry)
                    } else if(typeof(songEntry) == 'string') {
                        songItems.push(songEntry)
                    }
                }
                app.player.stop()
                app.player.playlist.assign(songItems)
                app.player.playPlaylist()
                if(songItems.length > 0) {
                    playbackStarted()
                }
            }
        }
    }
}