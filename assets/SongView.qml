import bb.cascades 1.1

Page {
    property alias songId: songIdField.text
    property alias songFileName: songFileNameField.text
    property alias songTitle: songTitleField.text
    property alias songFileSize: songFileSizeField.text
    property alias songMD5: songMD5Field.text
    
    property alias songOrders: songOrdersField.text
    property alias songPatterns: songPatternsField.text
    property alias songChannels: songChannelsField.text
    property alias songInstruments: songInstrumentsField.text
    property alias songSamples: songSamplesField.text
    
    objectName: "SongView"
    
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
            id: songMD5Field
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
    
    function load(songId) {
        var song = app.player.catalog.resolveModuleById(songId)
        songId = "ModID: " + song.id
        songFileName = "File Name: " + song.fileName
        songTitle = "Title: " + song.title
        songFileSize = "File Size: " + song.size
        songMD5 = "MD5: " + song.md5
        
        songOrders = "Orders: " + song.orders
        songPatterns = "Patterns: " + song.patterns
        songChannels = "Channels: " + song.channels
        songInstruments = "Instruments: " + song.instruments
        songSamples = "Samples: " + song.samples
    }
}
