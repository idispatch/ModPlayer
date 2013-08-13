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
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    
                    visible: app.player.currentSong.songLoaded
                    topPadding: 20
                    bottomPadding: 20
                    
                    LCDDisplay {
                        text: {
                            if(app.player.currentSong.title.length == 0) {
                                return app.player.currentSong.fileName
                            } else {
                                return app.player.currentSong.title
                            }
                        }
                        horizontalAlignment: HorizontalAlignment.Center
                    }

                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        ImageView {
                            verticalAlignment: VerticalAlignment.Center
                            imageSource: app.getIconPath(song)
                            preferredHeight: 128
                            preferredWidth: 128
                            minHeight: preferredHeight
                            maxHeight: preferredHeight
                            minWidth: preferredWidth 
                            maxWidth: preferredWidth
                        }
                        Container {
                            layout: StackLayout {
                                orientation: LayoutOrientation.TopToBottom
                            }
                            Label {
                                text: "File: " + app.player.currentSong.fileName
                            }
                            Label {
                                visible: app.player.currentSong.description.length > 0
                                text: "Description: " + app.player.currentSong.description
                            }
                            Label {
                                text: "Size: " + Global.getSizeKb(app.player.currentSong.fileSize)
                            }
                            Label {
                                text: "Length: " + app.player.currentSong.lengthTimeString
                            }
                        }
                    }

                    Divider {}
                    
                    SongStatData {
                        songLoaded: app.player.currentSong.songLoaded
                        songOrders: app.player.currentSong.orders
                        songChannels: app.player.currentSong.channels
                        songPatterns: app.player.currentSong.patterns
                        songInstruments: app.player.currentSong.instruments
                        songSamples: app.player.currentSong.samples
                    }
                    
                    Divider {}
                    
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
                    
                    Divider {
                    }
                }
                
                Label {
                    text: app.player.statusText
                }
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
