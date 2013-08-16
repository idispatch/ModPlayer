import bb.cascades 1.0
import "functions.js" as Global

Container {
    property variant song

    horizontalAlignment: HorizontalAlignment.Fill

    background: backgroundImagePaint.imagePaint

    topPadding: 12
    bottomPadding: 12
    leftPadding: 16
    rightPadding: 16

    topMargin: 16
    bottomMargin: 16

    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundImagePaint
            repeatPattern: RepeatPattern.Fill
            imageSource: "asset:///images/backgrounds/container_back.amd"
        }
    ]

    HorizontalContainer {
        visible: song!=null
        ImageView {
            imageSource: "asset:///images/badges/badge_played.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        Label {
            textFormat: TextFormat.Html
            text: {
                if(song!=null) {
                    if(song.playCount > 0) {
                        if(song.playCount == 1) {
                            return qsTr("You played this song once")
                        }
                        return "You played this song <b>" + song.playCount + "</b> times"
                    }
                    return qsTr("You did not play this song yet")
                }
                return "";
            }
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
        visible: song!=null && song.lastPlayed > 0
        ImageView {
            imageSource: "asset:///images/badges/badge_lastplayed.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        Label {
            text: song!=null && song.lastPlayed > 0 ? 
            "Last played " + Global.formatTimeStamp(song.lastPlayed) : 
            ""
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
        visible: song!=null && song.myFavourite > 0
        ImageView {
            imageSource: "asset:///images/badges/badge_myfavourite.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        Label {
            text: song != null && song.myFavourite > 0 ? qsTr("You liked this song") : ""
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
        visible: song!=null && app.cache.exists(song.fileName)
        textFormat: TextFormat.Html
        horizontalAlignment: HorizontalAlignment.Center
        text: getLabelText()
        function getLabelText() {
            if(song != null) {
                if(app.cache.exists(song.fileName)) {
                    return "<i>" + qsTr("You have this song in the cache already") + "</i>"
                }
            }
            return "";
        }
        function currentCacheFilesChanged() {
            visible = song!=null && app.cache.exists(song.fileName)
            text = getLabelText()
        }
        onCreationCompleted: {
            app.cache.currentFilesChanged.connect(currentCacheFilesChanged)
        }
    }
}
