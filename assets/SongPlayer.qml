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
                    VUMeter {
                        id: songVUMeter
                        topMargin: 10
                        horizontalAlignment: HorizontalAlignment.Center
                        visible: app.player.currentSong.songLoaded && app.player.state == Player.Playing
                        song: app.player.currentSong
                    }
                    Slider {
                        id: songProgress
                        visible: app.player.currentSong.songLoaded
                        fromValue: 0
                        toValue: app.player.currentSong.orders
                        value: app.player.currentSong.currentOrder
                        horizontalAlignment: HorizontalAlignment.Fill
                        onValueChanged: {
                            app.player.currentSong.seekToOrder(value)
                        }
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
               app.player.state == Player.Preparing) {
                progress.body = qsTr("Downloading song")
                progress.show()
            } else {
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
