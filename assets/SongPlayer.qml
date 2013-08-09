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
            }
            
            leftPadding: 20
            rightPadding: 20
            
            Label {
                id: statusText
                text: app.player.statusText
            }
            
            Divider {
            
            }
            LCD {
                text: app.player.currentSong.title
            }
            Label {
                text: "File: " + app.player.currentSong.fileName
                visible: app.player.currentSong.songLoaded
            }
            
            Label {
                text: "Size: " + Global.getSizeKb(app.player.currentSong.fileSize)
                visible: app.player.currentSong.songLoaded
            }
            
            Divider {
            
            }
            
            Label {
                text: "Title: " + app.player.currentSong.title
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Description: " + app.player.currentSong.description
                visible: app.player.currentSong.songLoaded && app.player.currentSong.description.length > 0
            }
            Label {
                text: "Length: " + app.player.currentSong.lengthTimeString
                visible: app.player.currentSong.songLoaded
            }
            Divider {
            
            }
            Label {
                text: "Channels: " + app.player.currentSong.channels
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Orders: " + app.player.currentSong.orders
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Patterns: " + app.player.currentSong.patterns
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Samples: " + app.player.currentSong.samples
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Instruments: " + app.player.currentSong.instruments
                visible: app.player.currentSong.songLoaded
            }
            Divider {}
            
            Label {
                text: "Current Order: " + app.player.currentSong.currentOrder
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Current Pattern: " + app.player.currentSong.currentPattern
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Current Row: " + app.player.currentSong.currentRow
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Current Speed: " + app.player.currentSong.currentSpeed
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Current Tempo: " + app.player.currentSong.currentTempo
                visible: app.player.currentSong.songLoaded
            }            
            Label {
                text: "Playing Channels: " + app.player.currentSong.playingChannels
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Master Volume: " + app.player.currentSong.masterVolume
                visible: app.player.currentSong.songLoaded
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
