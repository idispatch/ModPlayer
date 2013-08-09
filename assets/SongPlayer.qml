import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Page {
    property variant navigationPane
    property variant currentSong
    
    function play(song) {
        currentSong = song
        app.player.play(currentSong)
    }

    ScrollView {
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            }
            
            leftPadding: 20
            rightPadding: 20
            
            Label {
                text: app.player.statusText
            }
            
            Divider {
                visible: app.player.currentSong.songLoaded
            }
            
            LCDDisplay {
                visible: app.player.currentSong.songLoaded
                text: app.player.currentSong.title
            }
            
            Label {
                visible: app.player.currentSong.songLoaded
                text: "File: " + app.player.currentSong.fileName
                
            }
            Label {
                visible: app.player.currentSong.songLoaded && app.player.currentSong.description.length > 0
                text: "Description: " + app.player.currentSong.description
            }
            Label {
                visible: app.player.currentSong.songLoaded
                text: "Size: " + Global.getSizeKb(app.player.currentSong.fileSize)
            }
            Label {
                visible: app.player.currentSong.songLoaded
                text: "Length: " + app.player.currentSong.lengthTimeString
            }
            
            Divider {
                visible: app.player.currentSong.songLoaded
            }
            
            SongStatData {
                visible: app.player.currentSong.songLoaded
                songLoaded: app.player.currentSong.songLoaded
                songOrders: app.player.currentSong.orders
                songChannels: app.player.currentSong.channels
                songPatterns: app.player.currentSong.patterns
                songInstruments: app.player.currentSong.instruments
                songSamples: app.player.currentSong.samples
            }

            Divider {
                visible: app.player.currentSong.songLoaded
            }

            SongPlayData {
                visible: app.player.currentSong.songLoaded
                songLoaded: app.player.currentSong.songLoaded
                songOrder: app.player.currentSong.currentOrder
                songPattern: app.player.currentSong.currentPattern
                songRow: app.player.currentSong.currentRow
                songChannels: app.player.currentSong.playingChannels
                songSpeed: app.player.currentSong.currentSpeed
                songTempo: app.player.currentSong.currentTempo
                songVolume: app.player.currentSong.masterVolume
            }
        }
    }
    
    actions: [
        PlayActionItem {},
        PauseActionItem {}, 
        AddFavouriteActionItem {
            currentSong: app.player.currentSong.fileName
            enabled: app.player.currentSong.songLoaded
        },
        RemoveFavouriteActionItem {
            currentSong: app.player.currentSong.fileName
            enabled: app.player.currentSong.songLoaded
        }
    ]
}
