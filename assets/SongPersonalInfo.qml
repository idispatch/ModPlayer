import bb.cascades 1.0
import "functions.js" as Global

GroupContainer {
    property variant song
    HorizontalContainer {
        visible: song != null
        horizontalAlignment: HorizontalAlignment.Fill
        ImageView {
            imageSource: "asset:///images/badges/badge_played.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        Label {
            textFormat: TextFormat.Html
            text: {
                if(song) {
                    if(song.playCount > 0) {
                        if(song.playCount == 1) {
                            return qsTr("You played this song once")
                        }
                        return qsTr("You played this song <b>%1</b> times").arg(song.playCount)
                    }
                    return qsTr("You did not play this song yet")
                }
                return ""
            }
            textStyle.color: Color.Black
        }
        function songPlayCountChanged(){
            if(song.id == app.player.currentSong.id) {
                song.playCount = app.player.currentSong.playCount
            }
        }
        onCreationCompleted: {
            app.player.currentSong.playCountChanged.connect(songPlayCountChanged)
        }
    }
    HorizontalContainer {
        visible: song && song.lastPlayed > 0
        ImageView {
            imageSource: "asset:///images/badges/badge_lastplayed.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        Label {
            text: song && song.lastPlayed > 0 ? 
                qsTr("Last played %1").arg(Global.formatTimeStamp(song.lastPlayed)) : 
                ""
            textStyle.color: Color.Black
        }
        function songLastPlayedChanged(){
            if(song.id == app.player.currentSong.id) {
                song.lastPlayed = app.player.currentSong.lastPlayed
            }
        }
        onCreationCompleted: {
            app.player.currentSong.lastPlayedChanged.connect(songLastPlayedChanged)
        }
    }
    HorizontalContainer {
        visible: song && song.myFavourite > 0
        ImageView {
            imageSource: "asset:///images/badges/badge_myfavourite.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        Label {
            text: song && song.myFavourite > 0 ? qsTr("You liked this song") : ""
            textStyle.color: Color.Black
        }
        function songFavouriteChanged(){
            if(song.id == app.player.currentSong.id) {
                song.myFavourite = app.player.currentSong.myFavourite
            }
        }
        onCreationCompleted: {
            app.player.currentSong.myFavouriteChanged.connect(songFavouriteChanged)
        }
    }
    Label {
        visible: song && app.cache.exists(song.fileName)
        textFormat: TextFormat.Html
        horizontalAlignment: HorizontalAlignment.Center
        textStyle.color: Color.Black
        text: getLabelText()
        function getLabelText() {
            if(song) {
                if(app.cache.exists(song.fileName)) {
                    return qsTr("<i>You have this song in the cache already</i>")
                }
            }
            return ""
        }
        function currentCacheFilesChanged() {
            visible = song && app.cache.exists(song.fileName)
            text = getLabelText()
        }
        onCreationCompleted: {
            app.cache.currentFilesChanged.connect(currentCacheFilesChanged)
        }
    }
}
