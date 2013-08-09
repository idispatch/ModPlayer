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
            LCDDisplay {
                text: app.player.currentSong.title
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "File: " + app.player.currentSong.fileName
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Description: " + app.player.currentSong.description
                visible: app.player.currentSong.songLoaded && app.player.currentSong.description.length > 0
            }
            Label {
                text: "Size: " + Global.getSizeKb(app.player.currentSong.fileSize)
                visible: app.player.currentSong.songLoaded
            }
            Label {
                text: "Length: " + app.player.currentSong.lengthTimeString
                visible: app.player.currentSong.songLoaded
            }
            
            Divider {
            }
            
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
                        text: "Orders: " + app.player.currentSong.orders
                        visible: app.player.currentSong.songLoaded
                    }
                    Label {
                        text: "Patterns: " + app.player.currentSong.patterns
                        visible: app.player.currentSong.songLoaded
                    }
                    Label {
                        text: "Channels: " + app.player.currentSong.channels
                        visible: app.player.currentSong.songLoaded
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
                        text: "Instruments: " + app.player.currentSong.instruments
                        visible: app.player.currentSong.songLoaded
                    }
                    Label {
                        text: "Samples: " + app.player.currentSong.samples
                        visible: app.player.currentSong.songLoaded
                    }
                }
            }

            Divider {}

            SongPlayData {
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
