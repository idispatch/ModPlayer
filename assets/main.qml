import bb.cascades 1.2

Page {
    ScrollView {
        Container {
            layout: StackLayout {
            }
            
            leftPadding: 20
            rightPadding: 20
            
            TextField {
                id: modId
                text: ""
                hintText: "Module ID"
            }
            
            Label {
                id: statusText
                text: app.player.statusText
            }
            
            Divider {
                
            }
            
            Label {
                text: "File: " + app.player.currentSong.fileName
            }
            
            Label {
                text: "Size: " + app.player.currentSong.fileSize + " bytes"
            }
            
            Divider {
                
            }
            
            Label {
                text: "Title: " + app.player.currentSong.title
                visible: app.player.currentSong.title.length > 0
            }
            Label {
                text: "Description: " + app.player.currentSong.description
                visible: app.player.currentSong.description.length > 0
            }
            Label {
                text: "Channels: " + app.player.currentSong.channels
                visible: app.player.currentSong.channels > 0
            }
            Label {
                text: "Patterns: " + app.player.currentSong.patterns
                visible: app.player.currentSong.patterns > 0
            }
            Label {
                text: "Samples: " + app.player.currentSong.samples
                visible: app.player.currentSong.samples > 0
            }
            Label {
                text: "Instruments: " + app.player.currentSong.instruments
                visible: app.player.currentSong.instruments > 0
            }
            Divider {}
            
            Label {
                text: "Current Order: " + app.player.currentSong.currentOrder
                visible: app.player.currentSong.currentOrder > 0
            }
            Label {
                text: "Current Pattern: " + app.player.currentSong.currentPattern
                visible: app.player.currentSong.currentPattern > 0
            }
            Label {
                text: "Current Row: " + app.player.currentSong.currentRow
                visible: app.player.currentSong.currentRow > 0
            }
            Label {
                text: "Current Speed: " + app.player.currentSong.currentSpeed
                visible: app.player.currentSong.currentSpeed > 0
            }
            Label {
                text: "Current Tempo: " + app.player.currentSong.currentTempo
                visible: app.player.currentSong.currentTempo > 0
            }            
            Label {
                //property alias playingChannels: app.player.currentSong.playingChannels
                text: "Playing Channels: " + app.player.currentSong.playingChannels
                visible: app.player.currentSong.playingChannels > 0
            }
            Label {
                //property alias masterVolume: app.player.currentSong.masterVolume
                text: "Master Volume: " + app.player.currentSong.masterVolume
                visible: app.player.currentSong.masterVolume > 0
            }
            Divider {}
            Label {
                text: "Cached songs: " + app.player.cache.currentFiles
            }
            Label {
                text: {
                    var x = app.player.cache.currentSize
                    var s
                    if(x < 1024) {
                        s = x + " bytes"
                    } else { 
                        x /= 1024
                        if(x < 1024) {
                            s = x.toFixed(2) + " kB"
                        } else {
                            x /= 1024
                            s = x.toFixed(2) + " MB"
                        }
                    }
                    return "Used cache size: " + s 
                }
            }
            Divider {}
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Button {
                    text: "Play"
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        app.player.play(modId.text);
                    }
                }
                Button {
                    text: "Stop"
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        app.player.stop();
                    }
                }
            }
        }
    }
}
