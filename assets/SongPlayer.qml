import bb.cascades 1.1
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
        ActionItem {
            id: playStop
            title: {
                if(app.player.state == Player.Playing ||
                   app.player.state == Player.Paused) {
                    return "Stop"
                } else {
                    return "Play"
                }
            }
            enabled: app.player.currentSong.songLoaded
            imageSource: {
                if(app.player.state == Player.Playing ||
                   app.player.state == Player.Paused) {
                    return "asset:///images/icon_stop.png"
                } else {
                    return "asset:///images/icon_play.png"
                }
            }
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if(app.player.state == Player.Playing ||
                   app.player.state == Player.Paused) {
                    app.player.stop()
                } else {
                    app.player.play(currentSong)
                }
            }
        },
        ActionItem {
            id: pauseResume
            title: {
                if(app.player.state == Player.Paused) {
                    return "Resume"
                } else {
                    return "Pause"
                }
            }
            enabled: {
                return app.player.currentSong.songLoaded &&
                        (app.player.state == Player.Paused || 
                         app.player.state == Player.Playing);
            } 
            imageSource: "asset:///images/icon_pause.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if(app.player.state == Player.Paused) {
                    app.player.resume()
                } else if(app.player.state == Player.Playing) {
                    app.player.pause()
                }
            }
        }, 
        ActionItem {
            title: "Add to Favourites"
            enabled: app.player.currentSong.songLoaded
            imageSource: "asset:///images/icon_favorite.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            shortcuts: Shortcut {
                key: "f"
            } 
            onTriggered: {
                app.catalog.addFavourite(currentSong)
            }
        },
        ActionItem {
            title: "Remove from Favourites"
            enabled: app.player.currentSong.songLoaded
            imageSource: "asset:///images/icon_favorite_off.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            shortcuts: Shortcut {
                key: "u"
            } 
            onTriggered: {
                app.catalog.removeFavourite(currentSong)
            }
        }
    ]
}
