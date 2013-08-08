import bb.cascades 1.1

Page {
    property int moduleId
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
        }
    }
    
    function play() {
        var view = songPlayer.createObject()
        view.navigationPane = navigationPane 
        navigationPane.push(view)
        view.play(moduleId)
    }
    
    function showPlayer() {
        var view = songPlayer.createObject()
        view.navigationPane = navigationPane 
        navigationPane.push(view)
    }
    
    function load(songId) {
        console.log("load: " + songId);
        var song = app.player.catalog.resolveModuleById(songId)
        moduleId = song.id
        
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
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
    
    actions: [ 
        ActionItem {
             title: "Play"
             imageSource: "asset:///images/icon_play.png"
             enabled: moduleId!=0
             ActionBar.placement: ActionBarPlacement.OnBar
             shortcuts: Shortcut {
                 key: "p"
             } 
             onTriggered: {
                 play()
             }
        },
        ActionItem {
            title: "Player"
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
            title: "Add to Favourites"
            imageSource: "asset:///images/icon_favorite.png"
            enabled: moduleId!=0
            ActionBar.placement: ActionBarPlacement.InOverflow
            shortcuts: Shortcut {
                key: "f"
            } 
            onTriggered: {
                app.catalog.addFavourite(moduleId)
            }
        },
        ActionItem {
            title: "Remove from Favourites"
            imageSource: "asset:///images/icon_favorite_off.png"
            enabled: moduleId!=0
            ActionBar.placement: ActionBarPlacement.InOverflow
            shortcuts: Shortcut {
                key: "u"
            } 
            onTriggered: {
                app.catalog.removeFavourite(moduleId)
            }
        }
    ]
}
