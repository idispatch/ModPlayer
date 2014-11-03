import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

VerticalContainer {
    property string mode: "samples"
    function load() {
        if(mode == 'samples') {
            instrumentsSamplesList.dataModel = app.player.currentSong.getSampleNames()
        } else {
            instrumentsSamplesList.dataModel = app.player.currentSong.getInstrumentNames()
        }
    }
    onCreationCompleted: {
        var handler = function() {
            load()
        }
        selector.selectedIndexChanged.connect(handler)
        app.player.currentSong.songLoadedChanged.connect(handler)
    }
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
                mode = selector.selectedValue
            }
        }
    }
    GroupContainer {
        ListView {
            id: instrumentsSamplesList
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
        }
    }
}