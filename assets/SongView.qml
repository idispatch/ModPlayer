import bb.cascades 1.1

Page {
    property int moduleId
    property variant navigationPane 
    
    property alias songId: songIdField.text
    property alias songFileName: songFileNameField.text
    property alias songTitle: songTitleField.text
    property alias songFileSize: songFileSizeField.text
    //property alias songMD5: songMD5Field.text
    
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
            /*Label {
                id: songMD5Field
            }*/
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
            Divider {
            }
            
            Button {
                text: "Play"
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 40
                onClicked: play()
            }
        }
    }
    
    function play() {
        var player = songPlayer.createObject()
        navigationPane.push(player)
        player.play(moduleId)
    }
    
    function load(songId) {
        var song = app.player.catalog.resolveModuleById(songId)
        moduleId = song.id
        
        songId = "ModID: " + song.id
        songInCache = "Cached: " + (app.cache.exists(song.fileName) ? "yes" : "no") 
        
        songFileName = "File Name: " + song.fileName
        songTitle = "Title: " + song.title
        songFileSize = "File Size: " + song.size
        //songMD5 = "MD5: " + song.md5
        
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
             enabled: moduleId!=0
             shortcuts: Shortcut {
                 key: "p"
             } 
             //imageSource: "asset:///images/menuicons/icon_account.png"    
             onTriggered: {
                 play()
             }
        }
    ]
}
