import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Container {
    layout: DockLayout {}
    WallpaperView {}
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
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
                        text: app.title
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
                                    return qsTr("Playing") + Retranslate.onLanguageChanged
                                } else if (state == Player.Stopped) {
                                    return qsTr("Stopped") + Retranslate.onLanguageChanged
                                } else if (state == Player.Paused) {
                                    return qsTr("Paused") + Retranslate.onLanguageChanged
                                } else if (state == Player.Resolving) {
                                    return qsTr("Resolving") + Retranslate.onLanguageChanged
                                } else if (state == Player.Downloading) {
                                    return qsTr("Downloading") + Retranslate.onLanguageChanged
                                } else if (state == Player.Preparing) {
                                    return qsTr("Preparing") + Retranslate.onLanguageChanged
                                } else {
                                    return "---"
                                }
                            } else {
                                return qsTr("Idle") + Retranslate.onLanguageChanged
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
                        textStyle {
                            color: Color.White
                        }
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
                        text: {
                            var fileName
                            if(app.player.currentSong.isHttpSong) {
                                if(app.player.currentSong.fileName.indexOf("rockradio") != -1) {
                                    fileName = "www.rockradio.com"
                                } else if(app.player.currentSong.fileName.indexOf("jazzradio") != -1) {
                                    fileName = "www.jazzradio.com"
                                } else if(app.player.currentSong.fileName.indexOf("sky.fm") != -1) {
                                    fileName = "www.sky.fm"
                                } else if(app.player.currentSong.fileName.indexOf("fresca") != -1) {
                                    fileName = "www.frescaradio.com"
                                } else {
                                    fileName = "www.di.fm"
                                }
                            } else {
                                fileName = Global.fileNameOnly(app.player.currentSong.fileName)
                            }
                            return "(%1)".arg(fileName)
                        }
                        textStyle {
                            color: Color.White
                            fontSize: FontSize.XSmall
                        }
                    }
                }
            }
        }
    }
}