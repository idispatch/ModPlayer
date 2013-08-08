import bb.cascades 1.0
import "functions.js" as Global

Page {
    property variant song
    
    property variant navigationPane 
    
    property alias songId: songIdField.text
    property alias songFileName: songFileNameField.text
    property alias songTitle: songTitleField.text
    property alias songFileSize: songFileSizeField.text
    property alias songTracker: songTrackerField.text
    property alias songFormat: songFormatField.text
    
    property alias songOrders: songOrdersField.text
    property alias songPatterns: songPatternsField.text
    property alias songChannels: songChannelsField.text
    property alias songInstruments: songInstrumentsField.text
    property alias songSamples: songSamplesField.text
    
    property alias songDownloads: songDownloadsField.text
    property alias songFavourited: songFavouritedField.text
    property alias songScore: songScoreField.text
    
    property alias songInCache: songInCacheField.text
    
    property alias songPlayCount: songPlayCountField.text
    property alias songLastPlayed: songLastPlayedField.text
    property alias songMyFavorite: songMyFavouriteField.text
    
    objectName: "SongView"

    ScrollView {
        Container {
            layout: StackLayout {
            }
            
            leftPadding: 20
            rightPadding: 20
            
            Label {
                id: songIdField
            }
            Label {
                id: songFileNameField
            }
            Label {
                id: songFileSizeField
            }
            Label {
                id: songTitleField
            }
            Label {
                id: songFormatField
            }
            Label {
                id: songTrackerField
            }
            Divider{}
            Label {
                id: songPlayCountField
            }
            Label {
                id: songLastPlayedField
            }
            Label {
                id: songMyFavouriteField
            }
            Label {
                id: songInCacheField
            }
            Divider{}
            Label {
                id: songDownloadsField
            }
            Label {
                id: songFavouritedField
            }
            Label {
                id: songScoreField
            }
            Divider{}
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                horizontalAlignment: HorizontalAlignment.Fill
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Left
                    Label {
                        id: songOrdersField
                    }
                    Label {
                        id: songPatternsField
                    }
                    Label {
                        id: songChannelsField
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    
                    }
                    horizontalAlignment: HorizontalAlignment.Left
                    Label {
                        id: songInstrumentsField
                    }
                    Label {
                        id: songSamplesField
                    }
                }
            }
        }
    }
    
    function play() {
        var view = songPlayer.createObject()
        view.navigationPane = navigationPane 
        navigationPane.push(view)
        view.play(song.id)
    }
    
    function showPlayer() {
        var view = songPlayer.createObject()
        view.navigationPane = navigationPane 
        navigationPane.push(view)
    }
    
    function load(songId) {
        song = app.player.catalog.resolveModuleById(songId)
        
        songId = "ModID: " + song.id
        if(app.cache.exists(song.fileName)) {
            songInCache = "You have this song in the cache already"
        } else {
            songInCache = "You don't have this song in the cache yet"
        }
        
        songFileName = "File Name: " + song.fileName
        songTitle = "Title: " + song.title
        songFileSize = "File Size: " + Global.getSizeKb(song.size)
        
        songFormat = "Format: " + song.format
        songTracker = "Tracker: " + song.tracker
        
        songDownloads = "Dowloaded " + song.downloads + " times by others"
        if(song.favourited > 0) {
            songFavourited = "Favourited by " + song.favourited + " people"
        } else {
            songFavourited = "Not favourited by anyone yet"
        }
        
        if(song.score > 0) {
            songScore = "Rated " + song.score + " of 10 by others"
        } else {
            songScore = "Not rated by anyone yet"
        }
        
        songOrders = "Orders: " + song.orders
        songPatterns = "Patterns: " + song.patterns
        songChannels = "Channels: " + song.channels
        songInstruments = "Instruments: " + song.instruments
        songSamples = "Samples: " + song.samples
        
        if(song.playCount > 0) {
            if(song.playCount == 1) {
                songPlayCount = "You played this song once"
            } else {
                songPlayCount = "You played this song " + song.playCount + " times"
            }
            songLastPlayed = song.lastPlayed
        } else {
            songPlayCount = "You did not play this song yet"
            songLastPlayed = ""
        }
        
        if(song.myFavourite) {
            songMyFavorite = "You liked this song"
        } else {
            songMyFavorite = ""
        }
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
