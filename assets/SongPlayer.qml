import bb.cascades 1.1
import player 1.0

Page {
    function play(song) {
        app.player.play(song)
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
            
            Label {
                text: "File: " + app.player.currentSong.fileName
                visible: app.player.currentSong.songLoaded
            }
            
            Label {
                text: "Size: " + app.player.currentSong.fileSize + " bytes"
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
                //property alias playingChannels: app.player.currentSong.playingChannels
                text: "Playing Channels: " + app.player.currentSong.playingChannels
                visible: app.player.currentSong.songLoaded
            }
            Label {
                //property alias masterVolume: app.player.currentSong.masterVolume
                text: "Master Volume: " + app.player.currentSong.masterVolume
                visible: app.player.currentSong.songLoaded
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
                    //enabled: app.player.state == app.player.state.Stopped ||
                    //         app.player.state == app.player.state.Paused
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        //app.player.play(modId.text);
                    }
                }
                Button {
                    text: "Stop"
                    enabled: app.player.state == Player.Playing ||
                    app.player.state == Player.Paused
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        app.player.stop();
                    }
                }
                Button {
                    text: "Pause"
                    enabled: app.player.state == Player.Playing
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        app.player.pause();
                    }
                }
                Button {
                    text: "Resume"
                    enabled: app.player.state == Player.Paused 
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    onClicked: {
                        app.player.resume();
                    }
                }
            }
        }
    }
}
