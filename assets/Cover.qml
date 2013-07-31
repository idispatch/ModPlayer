import bb.cascades 1.0
import player 1.0

Container {
    background: Color.LightGray
    
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    
    layout: DockLayout {
    }

    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            text: "ModPlayer"
        }
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            text: {
                if(app.player.currentSong.songLoaded) {
                    var state = app.player.state;
                    if(state == Player.Playing) {
                        return "Playing";
                    } else if (state == Player.Stopped) {
                        return "Stopped";
                    } else if (state == Player.Paused) {
                        return "Paused";
                    } else {
                        return "";
                    }
                } else {
                    return "Idle";
                }
            }
        }
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            visible: app.player.currentSong.songLoaded && app.player.state == Player.Playing
            text: {
                if(app.player.currentSong.songLoaded) {
                    if(app.player.state == Player.Playing) {
                        return app.player.currentSong.fileName;
                    }
                }
                return "";
            }
        }
    }
}