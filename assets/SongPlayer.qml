import bb.cascades 1.0
import bb.system 1.0
import player 1.0
import "functions.js" as Global

Page {
    id: songPlayer
    objectName: "songPlayer"
    property variant navigationPane
    ViewContainer {
        VerticalContainer {
            horizontalAlignment: HorizontalAlignment.Fill
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
                    Container {
                        topMargin: 10
                        layout: DockLayout {}
                        Slider {
                            id: songProgress
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            visible: app.player.currentSong.songLoaded
                            fromValue: 0
                            toValue: app.player.currentSong.orders
                            value: app.player.currentSong.currentOrder
                            animations: [
                                FadeTransition {
                                    id: songProgressFadeIn
                                    duration: 1200
                                    easingCurve: StockCurve.CubicIn
                                    toOpacity: 1.0
                                },
                                FadeTransition {
                                    id: songProgressFadeOut
                                    duration: 1200
                                    easingCurve: StockCurve.CubicOut
                                    toOpacity: 0.0
                                }
                            ]
                        }
                        VUMeter {
                            id: songVUMeter
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            visible: app.player.currentSong.songLoaded
                            song: app.player.currentSong
                            opacity: 0.0
                            onTouch: {
                                songProgressFadeIn.stop()
                                songProgressFadeOut.stop()
                                songVUMeterFadeIn.stop()
                                songVUMeterFadeOut.stop()
                                
                                songVUMeter.opacity = 0.0
                                songProgress.opacity = 1.0
                            }
                            animations: [
                                FadeTransition {
                                    id: songVUMeterFadeIn
                                    duration: 1200
                                    easingCurve: StockCurve.CubicIn
                                    toOpacity: 1.0
                                },
                                FadeTransition {
                                    id: songVUMeterFadeOut
                                    duration: 1200
                                    easingCurve: StockCurve.CubicOut
                                    toOpacity: 0.0
                                }
                            ]
                        }
                        onCreationCompleted: {
                            viewTimer.start()
                        }
                        attachedObjects: [
                            QTimer {
                                id: viewTimer
                                interval: 4000
                                singleShot: false
                                onTimeout : {
                                    if(songProgress.opacity == 1.0) {
                                        if(app.player.state == Player.Playing) {
                                            songProgressFadeIn.stop()
                                            songVUMeterFadeOut.stop()
                                            
                                            songProgressFadeOut.play()
                                            songVUMeterFadeIn.play()
                                        } else {
                                            songProgressFadeOut.stop()
                                            songVUMeterFadeIn.stop()
                                            
                                            songProgressFadeIn.play()
                                            songVUMeterFadeOut.play()
                                        }
                                    } else {
                                        songProgressFadeOut.stop()
                                        songVUMeterFadeIn.stop()

                                        songProgressFadeIn.play()
                                        songVUMeterFadeOut.play()
                                    }
                                }                                        
                            }
                        ]
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
                            } else {
                                fadeAway.stop()
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
            }
        }
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            VerticalContainer {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                leftPadding: 20
                rightPadding: 20
                TGroupContainer {
                    topMargin: 16
                    bottomMargin: 16
                    topPadding: 16
                    bottomPadding: 16
                    leftPadding: 16
                    rightPadding: 16
                    visible: app.player.currentSong.songLoaded
                    SegmentedControl {
                        Option {
                            id: basicViewOption
                            text: qsTr("Basic")
                            selected: true
                        }
                        Option {
                            id: patternViewOption
                            text: qsTr("Pattern")
                        }
                        Option {
                            id: samplesViewOption
                            text: qsTr("Samples")
                        }
                        onSelectedOptionChanged: {
                            songMainInfo.visible = (selectedOption == basicViewOption)
                            songPublicInfo.visible = (selectedOption == basicViewOption)
                            songParametersInfo.visible = (selectedOption != samplesViewOption)
                            songDynamicInfo.visible = (selectedOption != samplesViewOption)
                            
                            patternView.visible = (selectedOption == patternViewOption)
                            if(selectedOption == samplesViewOption) {
                                instrumentsView.load()
                            }
                            instrumentsView.visible = (selectedOption == samplesViewOption)
                        }
                    }
                }
                VerticalContainer {
                    horizontalAlignment: HorizontalAlignment.Fill
                    SongMainInfo {
                        id: songMainInfo
                        song: app.player.currentSong
                        visible: app.player.currentSong.songLoaded
                    }
                    SongPublicInfo {
                        id: songPublicInfo
                        song: app.player.currentSong
                        visible: app.player.currentSong.songLoaded && app.player.currentSong.id != 0
                    }
                    SongParametersInfo {
                        id: songParametersInfo
                        song: app.player.currentSong
                        visible: app.player.currentSong.songLoaded
                    }
                    SongDynamicInfo {
                        id: songDynamicInfo
                        song: app.player.currentSong
                        visible: app.player.currentSong.songLoaded
                    }
                    TGroupContainer {
                        id: patternView
                        visible: false
                        PatternView {
                            song: app.player.currentSong
                            visible: app.player.currentSong.songLoaded
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
                    InstrumentsView {
                        id: instrumentsView
                        visible: false
                        maxHeight: 600
                        preferredHeight: 600
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
        PatternsActionItem {
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
