import bb.cascades 1.0

Page {
    property variant song
    
    property variant navigationPane 
    
    property alias songId: songIdField.text
    property alias songFileName: songFileNameField.text
    property alias songTitle: songTitleField.text
    property alias songFileSize: songFileSizeField.text
    
    property alias songOrders: songOrdersField.text
    property alias songPatterns: songPatternsField.text
    property alias songChannels: songChannelsField.text
    property alias songInstruments: songInstrumentsField.text
    property alias songSamples: songSamplesField.text
    
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
                id: songTitleField
            }
            Label {
                id: songFileSizeField
            }
            Label {
                id: songInCacheField
            }
            Label {
                id: songOrdersField
            }
            Label {
                id: songPatternsField
            }
            Label {
                id: songChannelsField
            }
            Label {
                id: songInstrumentsField
            }
            Label {
                id: songSamplesField
            }
            Label {
                id: songPlayCountField
            }
            Label {
                id: songLastPlayedField
            }
            Label {
                id: songMyFavouriteField
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
        songInCache = "Cached: " + (app.cache.exists(song.fileName) ? "yes" : "no") 
        
        songFileName = "File Name: " + song.fileName
        songTitle = "Title: " + song.title
        songFileSize = "File Size: " + song.size
        
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
