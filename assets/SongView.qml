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
                    text: {
                        if(song != null) {
                            return "File Name: " + song.fileName
                        }
                        return "";
                    }
                }
                Label {
                    text: {
                        if(song != null) {
                            return "File Size: " + Global.getSizeKb(song.fileSize)
                        }
                        return "";
                    }
                }
                Label {
                    text: {
                        if(song != null) {
                            return "Title: " + song.title
                        }
                        return "";
                    }
                }
                Label {
                    text: {
                        if(song != null) {
                            return "Format: " + song.format
                        }
                        return "";
                    }
                }
                Label {
                    text: {
                        if(song != null) {
                            return "Tracker: " + song.tracker
                        }
                        return "";
                    }
                }
                
                Divider{}
                
                Label {
                    text: {
                        if(song!=null) {
                            if(song.playCount > 0) {
                                if(song.playCount == 1) {
                                    return "You played this song once"
                                }
                                return "You played this song " + song.playCount + " times"
                            }
                            return "You did not play this song yet"
                        }
                        return "";
                    }
                }
                Label {
                    visible: song!=null && song.playCount > 0
                    text: {
                        if(song!=null) {
                            if(song.playCount > 0) {
                                return "Last played " + Global.formatTimeStamp(song.lastPlayed)
                            }
                        }
                        return "";
                    }
                }
                Label {
                    visible: song!=null && song.myFavourite > 0
                    text: {
                        if(song != null) {
                            if(song.myFavourite > 0) {
                                return "You liked this song"
                            }
                        }
                        return ""
                    }
                }
                Label {
                    visible: song!=null && app.cache.exists(song.fileName)
                    text: {
                        if(song != null) {
                            if(app.cache.exists(song.fileName)) {
                                return "You have this song in the cache already"
                            }
                        }
                        return "";
                    }
                }
                
                Divider{}
                
                Label {
                    text: {
                        if(song != null) {
                            return "Dowloaded " + song.downloads + " times by others"
                        }
                        return "";
                    }
                }
                Label {
                    text: {
                        if(song != null) {
                            if(song.favourited > 0) {
                                return "Favourited by " + song.favourited + " people"
                            }
                            return "Not favourited by anyone yet"
                        }
                        return "";
                    }
                }
                Label {
                    text: {
                        if(song != null) {
                            if(song.score > 0) {
                                return "Rated " + song.score + " of 10 by others"
                            }
                            return "Not rated by anyone yet"
                        }
                        return "";
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
