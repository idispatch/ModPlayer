import bb.cascades 1.0
import player 1.0

Container {
    property bool playing: app.player.currentSong.songLoaded && app.player.state == Player.Playing 

    layout: StackLayout {
        orientation: LayoutOrientation.TopToBottom
    }

    background: back.imagePaint
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill

    attachedObjects: [
        ImagePaintDefinition {
            id: back
            repeatPattern: RepeatPattern.Fill
            imageSource: "asset:///images/cover/cover.png"
        },
        ImagePaintDefinition {
            id: titleTextBack
            repeatPattern: RepeatPattern.Fill
            imageSource: "asset:///images/cover/cover_text_back.amd"
        }
    ]

    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }

        horizontalAlignment: HorizontalAlignment.Center
        topPadding: 30

        Container {
            background: titleTextBack.imagePaint
            horizontalAlignment: HorizontalAlignment.Center
            
            leftPadding: 15
            rightPadding: leftPadding
            
            topPadding: 5
            bottomPadding: topPadding
            
            Label {
                horizontalAlignment: HorizontalAlignment.Center
                text: "ModPlayer"
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    fontWeight: FontWeight.Bold
                    color: Color.White
                }
            }
        }
        
        Container {
            background: titleTextBack.imagePaint
            horizontalAlignment: HorizontalAlignment.Center
            
            topMargin: 50
            
            leftPadding: 15
            rightPadding: leftPadding
            topPadding: 5
            bottomPadding: topPadding
            
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
                textStyle {
                    color: Color.White
                }
            }
        }
        
        Container {
            background: titleTextBack.imagePaint
            horizontalAlignment: HorizontalAlignment.Center

            visible: playing
            
            leftPadding: 15
            rightPadding: leftPadding
            topPadding: 5
            bottomPadding: topPadding

            Label {
                horizontalAlignment: HorizontalAlignment.Center
                text: playing ? app.player.currentSong.title : ""
                textStyle {
                    color: Color.White
                }
            }
        }

        Container {
            background: titleTextBack.imagePaint
            horizontalAlignment: HorizontalAlignment.Center
            visible: playing
            
            topMargin: 30
            
            leftPadding: 15
            rightPadding: leftPadding
            topPadding: 5
            bottomPadding: topPadding

            Label {
                horizontalAlignment: HorizontalAlignment.Center
                text: playing ? "("+app.player.currentSong.fileName+")" : ""
                textStyle {
                    color: Color.White
                    fontSize: FontSize.XSmall
                }
            }
        }
    }
}