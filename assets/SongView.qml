import bb.cascades 1.0
import "functions.js" as Global

Page {
    property variant song
    property variant navigationPane 

    objectName: "SongView"
    Container {
        background: back.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.Fill
                imageSource: "asset:///images/background.png"
            }
        ]
        
        ScrollView {
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                
                leftPadding: 20
                rightPadding: 20
                topPadding: 20
                bottomPadding: 20
                
                visible: song != null
                
                Label {
                    text: song != null ? "File Name: <b>" + song.fileName + "</b>" : ""
                    textFormat: TextFormat.Html
                }
                Label {
                    text: song != null ? "File Size: <b>" + Global.getSizeKb(song.fileSize) + "</b>" : ""
                    textFormat: TextFormat.Html
                }
                Label {
                    text: song !== null ? "Title: <b>" + song.title + "</b>" : ""
                    textFormat: TextFormat.Html
                }
                Label {
                    text: song != null ? "Format: " + song.format : ""
                }
                Label {
                    text: song != null ? "Tracker: " + song.tracker : ""
                }

                Divider{}

                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    visible: song!=null
                    ImageView {
                        imageSource: "asset:///images/badge_played.png"
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                    }
                    Label {
                        textFormat: TextFormat.Html
                        text: {
                            if(song!=null) {
                                if(song.playCount > 0) {
                                    if(song.playCount == 1) {
                                        return "You played this song once"
                                    }
                                    return "You played this song <b>" + song.playCount + "</b> times"
                                }
                                return "You did not play this song yet"
                            }
                            return "";
                        }
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    visible: song!=null && song.playCount > 0
                    ImageView {
                        imageSource: "asset:///images/badge_lastplayed.png"
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                    }
                    Label {
                        text: {
                            if(song!=null) {
                                if(song.playCount > 0) {
                                    return "Last played " + Global.formatTimeStamp(song.lastPlayed)
                                }
                            }
                            return "";
                        }
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    visible: song!=null && song.myFavourite > 0
                    ImageView {
                        imageSource: "asset:///images/badge_myfavourite.png"
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                    }
                    Label {
                        text: {
                            if(song != null) {
                                if(song.myFavourite > 0) {
                                    return "You liked this song"
                                }
                            }
                            return ""
                        }
                    }
                }
                Label {
                    visible: song!=null && app.cache.exists(song.fileName)
                    textFormat: TextFormat.Html
                    text: {
                        if(song != null) {
                            if(app.cache.exists(song.fileName)) {
                                return "<i>You have this song in the cache already</i>"
                            }
                        }
                        return "";
                    }
                }

                Divider{}

                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    visible: song!=null
                    ImageView {
                        imageSource: "asset:///images/badge_downloads.png"
                        visible: song!=null && song.downloads > 0
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                    }
                    Label {
                        textFormat: TextFormat.Html
                        text: {
                            if(song != null) {
                                if(song.downloads > 0) {
                                    return "Dowloaded <b>" + song.downloads + "</b> times by others"
                                }
                            }
                            return "";
                        }
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    visible: song!=null
                    ImageView {
                        imageSource: "asset:///images/badge_favorites.png"
                        visible: song != null && song.favourited > 0
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                    }
                    Label {
                        textFormat: TextFormat.Html
                        text: {
                            if(song != null) {
                                if(song.favourited > 0) {
                                    return "Favourited by <b>" + song.favourited + "</b> people"
                                }
                                return "Not favourited by anyone yet"
                            }
                            return "";
                        }
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    visible: song!=null
                    ImageView {
                        imageSource: "asset:///images/badge_score.png"
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                        visible: song!=null && song.score > 0
                    }
                    Label {
                        textFormat: TextFormat.Html
                        text: {
                            if(song != null) {
                                if(song.score > 0) {
                                    return "Rated <b>" + song.score + "</b> of <b>10</b> by others"
                                }
                                return "Not rated by anyone yet"
                            }
                            return "";
                        }
                    }
                }

                Divider{}

                SongStatData {
                    songLoaded: true
                    songOrders: song == null ? -1 : song.orders
                    songChannels: song == null ? -1 : song.channels
                    songPatterns: song == null ? -1 : song.patterns
                    songInstruments: song == null ? -1 : song.instruments
                    songSamples: song == null ? -1 : song.samples
                }
            }
        }
    }

    function play() {
        if(song!=null) {
            var view = songPlayer.createObject()
            view.navigationPane = navigationPane 
            navigationPane.push(view)
            view.play(song.modId)
        }
    }
    
    function showPlayer() {
        var view = songPlayer.createObject()
        view.navigationPane = navigationPane 
        navigationPane.push(view)
    }
    
    function load(songId) {
        song = app.player.catalog.resolveModuleById(songId)
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
    
    actions: [ 
        ActionItem {
            title: "Now Playing"
            imageSource: "asset:///images/icon_player.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            shortcuts: Shortcut {
                key: "n"
            } 
            onTriggered: {
                showPlayer()
            }
        },
        ActionItem {
            title: "Play"
            imageSource: "asset:///images/icon_play.png"
            enabled: song != null
            ActionBar.placement: ActionBarPlacement.OnBar
            shortcuts: Shortcut {
                key: "p"
            } 
            onTriggered: {
                play()
            }
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AddFavouriteActionItem {
            currentSong: parent.song
        },
        RemoveFavouriteActionItem {
            currentSong: parent.song
        }
    ]
}
