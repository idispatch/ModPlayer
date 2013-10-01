import bb.cascades 1.0
import bb.system 1.0
import player 1.0
import "functions.js" as Global

Page {
    id: songPlayer
    objectName: "songPlayer"
    property variant navigationPane
    ViewContainer {
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            VerticalContainer {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                leftPadding: 20
                rightPadding: 20
                VerticalContainer {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: 20
                    bottomPadding: 20
                    TGroupContainer {
                        topMargin: 16
                        bottomMargin: 16
                        topPadding: 16
                        bottomPadding: 16
                        leftPadding: 16
                        rightPadding: 16 
                        background: vuPaint.imagePaint
                        horizontalAlignment: HorizontalAlignment.Center
                        LCDDisplay {
                            text: {
                                if(app.player.currentSong.title.length == 0) {
                                    return Global.fileNameOnly(app.player.currentSong.fileName)
                                } else {
                                    return app.player.currentSong.title
                                }
                            }
                            bottomMargin: 10
                            visible: app.player.currentSong.songLoaded
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                        VUMeter {
                            topMargin: 10
                            visible: app.player.currentSong.songLoaded && app.player.state == Player.Playing
                            song: app.player.currentSong
                        }
                        Label {
                            id: statusText
                            text: app.player.statusText
                            textStyle {
                                fontWeight: FontWeight.W100
                                fontSize: FontSize.Large
                                color: Color.White
                            }
                            onTextChanged: {
                                opacity = 1.0
                                visible = true
                                if(app.player.state == Player.Playing) {
                                    fadeAway.play()
                                }
                            }
                            animations: [
                                ParallelAnimation {
                                    id: fadeAway
                                    delay: 3000
                                    FadeTransition {
                                        duration: 2000
                                        easingCurve: StockCurve.CubicOut
                                        fromOpacity: 1.0
                                        toOpacity: 0.0
                                    }
                                    onEnded: {
                                        statusText.visible = false
                                    }
                                } 
                            ]
                        }
                        attachedObjects: [
                            ImagePaintDefinition {
                                id: vuPaint
                                repeatPattern: RepeatPattern.Fill
                                imageSource: "asset:///images/backgrounds/vu_back.amd"
                            }
                        ]
                    }
                    SongMainInfo {
                        song: app.player.currentSong
                        visible: app.player.currentSong.songLoaded
                    }
                    SongPublicInfo {
                        song: app.player.currentSong
                        visible: app.player.currentSong.songLoaded && app.player.currentSong.id != 0
                    }
                    SongParametersInfo {
                        song: app.player.currentSong
                        visible: app.player.currentSong.songLoaded
                    }
                    SongDynamicInfo {
                        song: app.player.currentSong
                        visible: app.player.currentSong.songLoaded
                    }
                }
            }
        }
    }
    onCreationCompleted: {
        app.player.stateChanged.connect(function() {
            if(app.player.state == Player.Downloading ||
               app.player.state == Player.Resolving ||
               app.player.state == Player.Preparing) 
            {
                progress.body = qsTr("Downloading song")
                progress.show()
            } 
            else 
            {
                progress.cancel()
            }
        })
    }
    attachedObjects: [
        ProgressToast {
            id: progress
        }
    ]
    actions: [
        PlayModeActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        PlayActionItem {},
        PauseActionItem {},
        SameArtistActionItem {
            currentSong: app.player.currentSong
            navigationPane: songPlayer.navigationPane 
        },
        InstrumentsActionItem {
            navigationPane: songPlayer.navigationPane
        },
        AddFavouriteActionItem {
            currentSong: app.player.currentSong
        },
        RemoveFavouriteActionItem {
            currentSong: app.player.currentSong
        },
        SaveSongActionItem{
            currentSong: app.player.currentSong.fileName
            enabled: app.player.currentSong.songLoaded && app.player.currentSong.id != 0
        },
        OpenSongActionItem{},
        AppWorldActionItem{}
    ]
}
