import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

GroupContainer {
    property variant song
    onCreationCompleted: {
        personalInfoAnimation.play()
    }
    animations: [
        ScaleTransition {
            id: personalInfoAnimation
            fromY: 0.0
            toY: 1.0
            duration: 400
            easingCurve: StockCurve.CubicInOut
        }
    ]
    HorizontalContainer {
        visible: song != null
        horizontalAlignment: HorizontalAlignment.Fill
        ImageView {
            imageSource: "asset:///images/badges/badge_played.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        BlackLabel {
            textFormat: TextFormat.Html
            multiline: true
            text: {
                if(song) {
                    if(song.playCount > 0) {
                        if(song.playCount == 1) {
                            return qsTr("You played this song once") + Retranslate.onLanguageChanged
                        }
                        return qsTr("You played this song <b>%1</b> times").arg(song.playCount) + Retranslate.onLanguageChanged
                    }
                    return qsTr("You did not play this song yet") + Retranslate.onLanguageChanged
                }
                return ""
            }
        }
        onCreationCompleted: {
            var thisSong = song
            app.player.currentSong.playCountChanged.connect(function(){
                if(thisSong.id == app.player.currentSong.id) {
                    thisSong.playCount = app.player.currentSong.playCount
                }
            })
        }
    }
    HorizontalContainer {
        visible: song && song.lastPlayed > 0
        ImageView {
            imageSource: "asset:///images/badges/badge_lastplayed.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        BlackLabel {
            text: song && song.lastPlayed > 0 ? 
            qsTr("Last played %1").arg(Global.formatTimeStamp(song.lastPlayed)) + Retranslate.onLanguageChanged : 
                ""
            multiline: true
        }
        onCreationCompleted: {
            var thisSong = song
            app.player.currentSong.lastPlayedChanged.connect(function(){
                if(thisSong && thisSong.id == app.player.currentSong.id) {
                    thisSong.lastPlayed = app.player.currentSong.lastPlayed
                }
            })
        }
    }
    HorizontalContainer {
        visible: song && song.myFavourite > 0
        ImageView {
            visible: song && song.myFavourite > 0
            imageSource: "asset:///images/badges/badge_myfavourite.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        ImageView {
            visible: song && song.myFavourite > 1
            imageSource: "asset:///images/badges/badge_myfavourite.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        ImageView {
            visible: song && song.myFavourite > 2
            imageSource: "asset:///images/badges/badge_myfavourite.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        ImageView {
            visible: song && song.myFavourite > 3
            imageSource: "asset:///images/badges/badge_myfavourite.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        ImageView {
            visible: song && song.myFavourite > 4
            imageSource: "asset:///images/badges/badge_myfavourite.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        BlackLabel {
            text: song && song.myFavourite > 0 ? qsTr("You liked this song") + Retranslate.onLanguageChanged : ""
            multiline: true
        }
        onCreationCompleted: {
            var thisSong = song
            app.player.currentSong.myFavouriteChanged.connect(function(){
                if(thisSong && thisSong.id == app.player.currentSong.id) {
                    thisSong.myFavourite = app.player.currentSong.myFavourite
                }
            })
        }
    }
    BlackLabel {
        id: songCacheStatus
        visible: song != null && (song.id < 0 || app.cache.fileExists(song.fileName))
        textFormat: TextFormat.Html
        horizontalAlignment: HorizontalAlignment.Center
        multiline: true
        text: getLabelText()
        function getLabelText() {
            if(song) {
                if(song.id < 0) {
                    var fileName = String(song.fileName).replace(/^\/accounts\/1000\/(?:removable|shared)/,'')
                    return "<i>%1</i>".arg(Global.escapeHtml(fileName))
                } else {
                    if(app.cache.fileExists(song.fileName)) {
                        return qsTr("<i>You have this song in the cache already</i>") + Retranslate.onLanguageChanged
                    }
                }
            }
            return ""
        }
        onCreationCompleted: {
            var thisSong = song
            var thisObject = songCacheStatus
            app.cache.currentFilesChanged.connect(function() {
                thisObject.visible = thisSong && app.cache.fileExists(thisSong.fileName)
                thisObject.text = thisObject.getLabelText()
            })
        }
    }
}
