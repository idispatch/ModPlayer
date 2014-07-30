import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: liveStreamRadioPage
    objectName: "liveStreamRadioPage"
    property variant navigationPane
    property variant selectedRadio
    property int limit: 200
    property int requestId

    titleBar: PlayerTitleBar {
        id: titleBar
        title: {
            var c = radioList.dataModel ? radioList.dataModel.size() : 0
            if(c > limit) {
                c = " (%1+)".arg(limit)
            } else {
                c = " (%1)".arg(c)
            }
            qsTr("Select Internet Radio Channel") + c
        }
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
                            Option {
                                text: "All European Radio Stations"
                                selected: true
                            }
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
            id: radioList
            visible: !progress.running
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
                        description: ListItem.data.location + ", " + ListItem.data.country
                        text: ListItem.data.style
                        upperStatus: "Mp3"
                        middleStatus: "%1 kBps".arg(ListItem.data.bitrate)
                        imageSource: ListItem.data.flag
                    }
                }
            ]
            onTriggered: {
                selectedRadio = dataModel.data(indexPath)
                app.player.statusText = qsTr("Tuning Internet Radio")
                app.player.currentSong.title = "Internet Radio";
                showPlayerView()
                app.analytics.selectRadio(selectedRadio.radioPlaylist)
                app.player.radio.download(selectedRadio.radioPlaylist)
            }
        }
    }
    function unload() {
        progress.start()
        if(radioList.dataModel) {
            radioList.dataModel.clear()
        }
        radioList.resetDataModel()
    }
    function load() {
        unload()
        var countrySelection = ""
        if(country.selectedOption) {
            countrySelection = country.selectedOption.text
            if(countrySelection == "All European Radio Stations") {
                countrySelection = ""
            }
        }
        requestId = app.player.catalog.findLiveStreamRadioAsync(searchArea.searchTerm, 
                                                                countrySelection, 
                                                                limit)
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
             radioList.visible = true
             radioList.dataModel = result
        })
        app.player.radio.downloadFinished.connect(function(playlist,result) {
            if(result.length > 0) {
                app.analytics.playRadio(result[0])
                var flag = ""
                if(selectedRadio) {
                    flag = selectedRadio.flag
                }
                app.player.playRadio(result[0], flag)
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
