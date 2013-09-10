import bb.cascades 1.0

Page {
    id: instrumentsView
    property string mode: "samples"
    titleBar: PlayerTitleBar {
        title: {
            if(app.player.currentSong.songLoaded) {
                if(mode == 'samples') {
                    return qsTr("Samples of %1 (%2)").arg(app.player.currentSong.fileName).arg(app.player.currentSong.samples)
                } else {
                    return qsTr("Instruments of %1 (%2)").arg(app.player.currentSong.fileName).arg(app.player.currentSong.instruments)
                }
            } else {
                return qsTr("Instruments and Samples")
            }
        }
    }
    ViewContainer {
        VerticalContainer {
            leftPadding: 20
            rightPadding: 20
            topPadding: 20
            bottomPadding: 20
            GroupContainer {
                SegmentedControl {
                    id: selector
                    Option {
                        text: qsTr("Samples")
                        value: "samples"
                        enabled: app.player.currentSong.songLoaded && app.player.currentSong.samples > 0
                        selected: app.player.currentSong.samples > 0
                    }
                    Option {
                        text: qsTr("Instruments")
                        value: "instruments"
                        enabled: app.player.currentSong.songLoaded && app.player.currentSong.instruments > 0
                    }
                    onSelectedIndexChanged: {
                        instrumentsView.mode = selector.selectedValue
                    }
                }
            }
            GroupContainer {
                VerticalContainer {
                    horizontalAlignment: HorizontalAlignment.Fill
                    ListView {
                        id: itemList
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                        topPadding: 20
                        bottomPadding: topPadding
                        leftPadding: 10
                        rightPadding: leftPadding
                        listItemComponents: [
                            ListItemComponent {
                                VerticalContainer{
                                    id: row
                                    HorizontalContainer {
                                        horizontalAlignment: HorizontalAlignment.Fill
                                        HorizontalContainer {
                                            preferredWidth: 60
                                            minWidth: 60
                                            BlackLabel {
                                                text: row.ListItem.data['id']
                                                textStyle {
                                                    base: SystemDefaults.TextStyles.BodyText
                                                    fontWeight: FontWeight.W100
                                                    color: Color.Black
                                                }
                                            }
                                        }
                                        BlackLabel {
                                            text: row.ListItem.data['name']
                                        }
                                    }
                                    Divider {}
                                }
                            }
                        ]
                        function load() {
                            if(mode == 'samples') {
                                dataModel = app.player.currentSong.getSampleNames()
                            } else {
                                dataModel = app.player.currentSong.getInstrumentNames()
                            }
                        }
                        onCreationCompleted: {
                            selector.selectedIndexChanged.connect(load)
                            mode = 'samples'
                            load()
                        }
                    }
                }
            }
        }
    }
    actions: [ 
        PlayActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        AddFavouriteActionItem {
            currentSong: app.player.currentSong.songLoaded
        },
        RemoveFavouriteActionItem {
            currentSong: app.player.currentSong.songLoaded
        },
        AppWorldActionItem{}
    ]
}
