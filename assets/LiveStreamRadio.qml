import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: liveStreamRadioPage
    objectName: "liveStreamRadioPage"
    property variant navigationPane
    property int limit: 500
    property int requestId

    property string country: ""

    titleBar: PlayerTitleBar {
        id: titleBar
        title: qsTr("Select Internet Radio Channel")
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            HorizontalContainer {
                leftPadding: 10
                Label {
                    text: titleBar.title
                    textStyle {
                        color: Color.White 
                        fontSize: FontSize.Large
                    }
                    verticalAlignment: VerticalAlignment.Center
                }
            }
            expandableArea {
                content: VerticalContainer {
                    topPadding: 20.0
                    HorizontalContainer {
                        leftPadding: 20.0
                        rightPadding: 20.0
                        DropDown {
                            id: country
                            title: qsTr("Country")
                            leftPadding: 10
                            rightPadding: 10
                            topPadding: 10
                            bottomPadding: 10
                            Option {
                                text: "Armenia"
                            }
                            Option {
                                text: "Austria"
                            }
                            Option {
                                text: "Azerbaijan"
                            }
                            Option {
                                text: "Belarus"
                            }
                            Option {
                                text: "Belgium"
                            }
                            Option {
                                text: "Bosnia-Herzegovina"
                            }
                            Option {
                                text: "Bulgaria"
                            }
                            Option {
                                text: "Croatia"
                            }
                            Option {
                                text: "Cyprus"
                            }
                            Option {
                                text: "Czech Republic"
                            }
                            Option {
                                text: "Denmark"
                            }
                            Option {
                                text: "Estonia"
                            }
                            Option {
                                text: "Faroe Islands"
                            }
                            Option {
                                text: "Finland"
                            }
                            Option {
                                text: "France"
                            }
                            Option {
                                text: "Georgia"
                            }
                            Option {
                                text: "Germany"
                            }
                            Option {
                                text: "Gibraltar"
                            }
                            Option {
                                text: "Greece"
                            }
                            Option {
                                text: "Hungary"
                            }
                            Option {
                                text: "Iceland"
                            }
                            Option {
                                text: "Ireland"
                            }
                            Option {
                                text: "Italy"
                            }
                            Option {
                                text: "Kosovo"
                            }
                            Option {
                                text: "Latvia"
                            }
                            Option {
                                text: "Liechtenstein"
                            }
                            Option {
                                text: "Lithuania"
                            }
                            Option {
                                text: "Luxembourg"
                            }
                            Option {
                                text: "Macedonia"
                            }
                            Option {
                                text: "Malta"
                            }
                            Option {
                                text: "Moldova"
                            }
                            Option {
                                text: "Monaco"
                            }
                            Option {
                                text: "Montenegro"
                            }
                            Option {
                                text: "Netherlands"
                            }
                            Option {
                                text: "Norway"
                            }
                            Option {
                                text: "Poland"
                            }
                            Option {
                                text: "Portugal"
                            }
                            Option {
                                text: "Romania"
                            }
                            Option {
                                text: "Russia"
                            }
                            Option {
                                text: "Serbia"
                            }
                            Option {
                                text: "Slovakia"
                            }
                            Option {
                                text: "Slovenia"
                            }
                            Option {
                                text: "Spain"
                            }
                            Option {
                                text: "Sweden"
                            }
                            Option {
                                text: "Switzerland"
                            }
                            Option {
                                text: "Turkey"
                            }
                            Option {
                                text: "Ukraine"
                            }
                            Option {
                                text: "United Kingdom"
                            }
                            onSelectedValueChanged: {
                                load()
                            }
                        }
                    }
                    SearchArea {
                        id: searchArea
                        hintText: qsTr("search stations")
                        onSearch: {
                            load()
                        }
                    }
                }
                expanded: true
                indicatorVisibility: TitleBarExpandableAreaIndicatorVisibility.Visible
                toggleArea: TitleBarExpandableAreaToggleArea.EntireTitleBar
            }
        }
    }
    ViewContainer {
        ProgressComponent {
            id: progress
        }
        ListView {
            id: liveStreamRadioList
            visible: ! progress.running
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            topPadding: 20
            bottomPadding: topPadding
            leftPadding: 10
            rightPadding: leftPadding
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Label {
                        text: ListItem.data
                        textStyle {
                            fontWeight: FontWeight.Bold
                            fontSize: FontSize.Large
                            color: Color.White
                        }
                    }
                },
                ListItemComponent {
                    type: "item"
                    ModPlayerListItem {
                        title: ListItem.data.name
                        description: ListItem.data.style
                        middleStatus: ListItem.data.country
                        lowerStatus: ListItem.data.location
                        imageSource: ListItem.data.flag
                    }
                }
            ]
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                app.player.statusText = qsTr("Tuning Internet Radio")
                app.player.currentSong.title = "Internet Radio";
                showPlayerView()
                app.analytics.selectRadio(chosenItem.radioPlaylist)
                app.player.radio.download(chosenItem.radioPlaylist)
            }
        }
    }
    function unload() {
        progress.start()
        if(liveStreamRadioList.dataModel) {
            liveStreamRadioList.dataModel.clear()
        }
        liveStreamRadioList.resetDataModel()
    }
    function load() {
        unload()
        requestId = app.player.catalog.findLiveStreamRadioAsync(searchArea.searchTerm, country, limit)
    }
    function showPlayerView() {
        if(mainTabPane.activePane == navigationPane && 
           navigationPane.top == liveStreamRadioPage) {
            var view = songPlayer.createObject()
            view.navigationPane = navigationPane
            navigationPane.push(view)
        }
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(function() {
                if(mainTabPane.activePane == navigationPane && 
                   navigationPane.top == liveStreamRadioPage) {
                    var view = songPlayer.createObject()
                    view.navigationPane = navigationPane
                    navigationPane.push(view)
                }
        })
        app.catalog.resultReady.connect(function(responseId, result) {
                if(responseId != requestId) 
                    return
                requestId = 0
                progress.stop()
                liveStreamRadioList.visible = true
                liveStreamRadioList.dataModel = result
        })
        app.player.radio.downloadFinished.connect(function(url,result) {
            if(result.length > 0) {
                app.analytics.playRadio(result[0])
                app.player.play(result[0])
            }
    })
    }
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
    actions: [
        PlayerActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            navigationPane: liveStreamRadioPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            navigationPane: liveStreamRadioPage.navigationPane
        },
        ImportSongsActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        OpenSongActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
}
