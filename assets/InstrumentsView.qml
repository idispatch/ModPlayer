import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

VerticalContainer {
    id: instrumentsView
    property string mode: "samples"
    function load() {
        if(mode == 'samples') {
            instrumentsSamplesList.dataModel = app.player.currentSong.getSampleNames()
        } else {
            instrumentsSamplesList.dataModel = app.player.currentSong.getInstrumentNames()
        }
    }
    onCreationCompleted: {
        var thisInstrumentsView = instrumentsView
        selector.selectedIndexChanged.connect(function() {
            thisInstrumentsView.load()
        })
        app.player.currentSong.songLoadedChanged.connect(function() {
            thisInstrumentsView.load()
        })
        thisInstrumentsView.load()
    }
    GroupContainer {
        SegmentedControl {
            id: selector
            Option {
                text: qsTr("Samples") + Retranslate.onLanguageChanged
                value: "samples"
                enabled: app.player.currentSong.songLoaded && app.player.currentSong.samples > 0
                selected: app.player.currentSong.samples > 0
            }
            Option {
                text: qsTr("Instruments") + Retranslate.onLanguageChanged
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