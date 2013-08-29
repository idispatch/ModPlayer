import bb.cascades 1.0
import player 1.0

ViewContainer {
    VerticalContainer {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        attachedObjects: [
            ImagePaintDefinition {
                id: titleTextBack
                repeatPattern: RepeatPattern.Fill
                imageSource: "asset:///images/cover/cover_text_back.amd"
            }
        ]
        VerticalContainer {
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
                topMargin: 35
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
                                return qsTr("Playing")
                            } else if (state == Player.Stopped) {
                                return qsTr("Stopped")
                            } else if (state == Player.Paused) {
                                return qsTr("Paused")
                            } else if (state == Player.Resolving) {
                                return qsTr("Resolving")
                            } else if (state == Player.Downloading) {
                                return qsTr("Downloading")
                            } else if (state == Player.Preparing) {
                                return qsTr("Preparing")
                            } else {
                                return "---"
                            }
                        } else {
                            return qsTr("Idle")
                        }
                    }
                    textStyle.color: Color.White
                }
            }
            Container {
                background: titleTextBack.imagePaint
                horizontalAlignment: HorizontalAlignment.Center
                visible: {
                    return app.player.currentSong.songLoaded && 
                            (app.player.state == Player.Playing || 
                             app.player.state == Player.Paused || 
                             app.player.state == Player.Stopped);
                } 
                leftPadding: 15
                rightPadding: leftPadding
                topPadding: 5
                bottomPadding: topPadding
                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    text: visible ? app.player.currentSong.title : ""
                    textStyle.color: Color.White
                }
            }
            Container {
                background: titleTextBack.imagePaint
                horizontalAlignment: HorizontalAlignment.Center
                visible: {
                    return app.player.currentSong.songLoaded && 
                    (app.player.state == Player.Playing || 
                     app.player.state == Player.Paused || 
                     app.player.state == Player.Stopped);
                }
                topMargin: 30
                leftPadding: 15
                rightPadding: leftPadding
                topPadding: 5
                bottomPadding: topPadding
                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    text: visible ? "("+app.player.currentSong.fileName+")" : ""
                    textStyle.color: Color.White
                    textStyle.fontSize: FontSize.XSmall
                }
            }
        }
    }
}