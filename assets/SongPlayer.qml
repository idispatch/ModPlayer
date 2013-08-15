import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Page {
    property variant navigationPane

    Container {
        background: back.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.Fill
                imageSource: "asset:///images/backgrounds/background.png"
            }
        ]
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill

        ScrollView {
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }

                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

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
                        SongIconView {
                            song: app.player.currentSong
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
                                text: "Length: " + app.player.currentSong.songLengthText
                            }
                        }
                    }

                    Divider {}

                    SongOtherStats {
                        song: app.player.currentSong
                    }

                    Divider {}

                    SongStatData {
                        song: app.player.currentSong
                    }
                    
                    Divider {}
                    
                    SongPlayData {
                        song: app.player.currentSong
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
