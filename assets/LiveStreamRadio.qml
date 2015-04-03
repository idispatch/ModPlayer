import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

Page {
    id: liveStreamRadioPage

    property variant navigationPane
    property variant selectedRadio
    property string playlistURL
    property int limit: 1000
    property int requestId

    titleBar: PlayerTitleBar {
        id: titleBar
        title: {
            var numItems = radioList.dataModel ? radioList.dataModel.size() : 0
            if(numItems > limit) {
                numItems = " (%1+)".arg(limit)
            } else {
                numItems = " (%1)".arg(numItems)
            }
            return (qsTr("Select Internet Radio Channel") + Retranslate.onLanguageChanged) + numItems
        }
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            TitleBarText {
                title: titleBar.title
            }
            expandableArea {
                content: VerticalContainer {
                    topPadding: 20.0
                    HorizontalContainer {
                        leftPadding: 20.0
                        rightPadding: 20.0
                        DropDown {
                            id: country
                            title: qsTr("Country") + Retranslate.onLanguageChanged
                            Option {
                                text: "All European Radio Stations"
                                selected: true
                            }
                            Option {
                                text: "Armenia"
                                imageSource: "asset:///images/flags/Armenia-flag.png"
                            }
                            Option {
                                text: "Austria"
                                imageSource: "asset:///images/flags/Austria-flag.png"
                            }
                            Option {
                                text: "Azerbaijan"
                                imageSource: "asset:///images/flags/Azerbaijan-flag.png"
                            }
                            Option {
                                text: "Belarus"
                                imageSource: "asset:///images/flags/Belarus-flag.png"
                            }
                            Option {
                                text: "Belgium"
                                imageSource: "asset:///images/flags/Belgium-flag.png"
                            }
                            Option {
                                text: "Bosnia-Herzegovina"
                                imageSource: "asset:///images/flags/Bosnian-flag.png"
                            }
                            Option {
                                text: "Bulgaria"
                                imageSource: "asset:///images/flags/Bulgaria-flag.png"
                            }
                            Option {
                                text: "Croatia"
                                imageSource: "asset:///images/flags/Croatian-flag.png"
                            }
                            Option {
                                text: "Cyprus"
                                imageSource: "asset:///images/flags/Cyprus-flag.png"
                            }
                            Option {
                                text: "Czech Republic"
                                imageSource: "asset:///images/flags/Czech-republic.png"
                            }
                            Option {
                                text: "Denmark"
                                imageSource: "asset:///images/flags/Denmark.png"
                            }
                            Option {
                                text: "Estonia"
                                imageSource: "asset:///images/flags/Estonia.png"
                            }
                            Option {
                                text: "Faroe Islands"
                                imageSource: "asset:///images/flags/Faroe-islands.png"
                            }
                            Option {
                                text: "Finland"
                                imageSource: "asset:///images/flags/Finland-flag.png"
                            }
                            Option {
                                text: "France"
                                imageSource: "asset:///images/flags/France-flag.png"
                            }
                            Option {
                                text: "Georgia"
                                imageSource: "asset:///images/flags/Georgia-flag.png"
                            }
                            Option {
                                text: "Germany"
                                imageSource: "asset:///images/flags/Germany-flag.png"
                            }
                            Option {
                                text: "Gibraltar"
                                imageSource: "asset:///images/flags/Gibraltar-flag.png"
                            }
                            Option {
                                text: "Greece"
                                imageSource: "asset:///images/flags/Greece-flag.png"
                            }
                            Option {
                                text: "Hungary"
                                imageSource: "asset:///images/flags/Hungary-flag.png"
                            }
                            Option {
                                text: "Iceland"
                                imageSource: "asset:///images/flags/Iceland-flag.png"
                            }
                            Option {
                                text: "Ireland"
                                imageSource: "asset:///images/flags/Ireland-flag.png"
                            }
                            Option {
                                text: "Italy"
                                imageSource: "asset:///images/flags/Italy-flag.png"
                            }
                            Option {
                                text: "Kosovo"
                                imageSource: "asset:///images/flags/Kosovo.png"
                            }
                            Option {
                                text: "Latvia"
                                imageSource: "asset:///images/flags/Latvia-flag.png"
                            }
                            Option {
                                text: "Liechtenstein"
                                imageSource: "asset:///images/flags/Liechtenstein-flag.png"
                            }
                            Option {
                                text: "Lithuania"
                                imageSource: "asset:///images/flags/Lithuania-flag.png"
                            }
                            Option {
                                text: "Luxembourg"
                                imageSource: "asset:///images/flags/Luxembourg-flag.png"
                            }
                            Option {
                                text: "Macedonia"
                                imageSource: "asset:///images/flags/Macedonia-flag.png"
                            }
                            Option {
                                text: "Malta"
                                imageSource: "asset:///images/flags/Malta-flag.png"
                            }
                            Option {
                                text: "Moldova"
                                imageSource: "asset:///images/flags/Moldova-flag.png"
                            }
                            Option {
                                text: "Monaco"
                                imageSource: "asset:///images/flags/Monaco-flag.png"
                            }
                            Option {
                                text: "Montenegro"
                                imageSource: "asset:///images/flags/Montenegro.png"
                            }
                            Option {
                                text: "Netherlands"
                                imageSource: "asset:///images/flags/Netherlands-flag.png"
                            }
                            Option {
                                text: "Norway"
                                imageSource: "asset:///images/flags/Norway-flag.png"
                            }
                            Option {
                                text: "Poland"
                                imageSource: "asset:///images/flags/Poland-flag.png"
                            }
                            Option {
                                text: "Portugal"
                                imageSource: "asset:///images/flags/Portugal-flag.png"
                            }
                            Option {
                                text: "Romania"
                                imageSource: "asset:///images/flags/Romania-flag.png"
                            }
                            Option {
                                text: "Russia"
                                imageSource: "asset:///images/flags/Russia-flag.png"
                            }
                            Option {
                                text: "Serbia"
                                imageSource: "asset:///images/flags/Serbia-flag.png"
                            }
                            Option {
                                text: "Slovakia"
                                imageSource: "asset:///images/flags/Slovakia-flag.png"
                            }
                            Option {
                                text: "Slovenia"
                                imageSource: "asset:///images/flags/Slovenia-flag.png"
                            }
                            Option {
                                text: "Spain"
                                imageSource: "asset:///images/flags/Spain-flag.png"
                            }
                            Option {
                                text: "Sweden"
                                imageSource: "asset:///images/flags/Sweden-flag.png"
                            }
                            Option {
                                text: "Switzerland"
                                imageSource: "asset:///images/flags/Switzerland-flag.png"
                            }
                            Option {
                                text: "Turkey"
                                imageSource: "asset:///images/flags/Turkey-flag.png"
                            }
                            Option {
                                text: "Ukraine"
                                imageSource: "asset:///images/flags/Ukraine-flag.png"
                            }
                            Option {
                                text: "United Kingdom"
                                imageSource: "asset:///images/flags/United-kingdom-flag.png"
                            }
                            onSelectedValueChanged: {
                                load()
                            }
                        }
                    }
                    SearchArea {
                        id: searchArea
                        hintText: qsTr("search stations") + Retranslate.onLanguageChanged
                        onSearch: {
                            liveStreamRadioPage.load()
                        }
                    }
                }
                expanded: true
                indicatorVisibility: TitleBarExpandableAreaIndicatorVisibility.Visible
                toggleArea: TitleBarExpandableAreaToggleArea.EntireTitleBar
            }
        }
    }
    Container {
        layout: DockLayout {}
        WallpaperView {}
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
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
                            favourite: false
                            description: ListItem.data.location + ", " + ListItem.data.country
                            text: ListItem.data.url
                            upperStatus: "Mp3"
                            middleStatus: ListItem.data.bitrate > 0 ? "%1 kBps".arg(ListItem.data.bitrate) : ""
                            lowerStatus: ListItem.data.style
                            imageSource: ListItem.data.flag
                        }
                    }
                ]
                onTriggered: {
                    selectedRadio = dataModel.data(indexPath)
                    playlistURL = selectedRadio.radioPlaylist
                    app.player.statusText = qsTr("Tuning Internet Radio") + Retranslate.onLanguageChanged
                    app.player.currentSong.title = "Internet Radio";
                    showPlayer()
                    app.analytics.selectRadio(playlistURL)
                    app.player.radio.download(playlistURL)
                }
            }
        }
        ListViewItemIndicator {
            id: indicatorView
        }
        ListViewItemSelector {
            id: indexLettersView
            onItemSelected: {
                indicatorView.text = item
                var indexPath = dataModel.lowerBound([item])
                indexPath.pop()
                radioList.scrollToItem(indexPath, app.scrollAnimationType)
            }
        }
        SleepTimerDisplay {}
        PlaylistControl {}
    }
     function unload() {
         progress.start()
         if(radioList.dataModel) {
             radioList.dataModel.clear()
         }
         indexLettersView.dataModel = null
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
     function showPlayer() {
         var view = songPlayer.createObject()
         if(view) {
             view.navigationPane = navigationPane
             navigationPane.push(view)
         }
     }
     function addBuyButton() {
         if(!app.isExtendedVersion) {
             var buyActionItem = buyAppComponentDefinition.createObject()
             if(buyActionItem) {
                 addAction(buyActionItem, ActionBarPlacement.InOverflow)
             }
         }
     }
     onCreationCompleted: {
         var thisMainTabPane = mainTabPane
         var thisObject = liveStreamRadioPage
         app.player.requestPlayerView.connect(function() {
            if(thisMainTabPane.activePane == thisObject.navigationPane && thisObject.navigationPane.top == thisObject) {
                thisObject.showPlayer()
            }
         })
         var thisProgress = progress
         var thisRadioList = radioList
         var thisIndexLettersView = indexLettersView
         app.catalog.resultReady.connect(function(responseId, result) {
              if(responseId == thisObject.requestId) {
                  thisObject.requestId = 0
                  thisProgress.stop()
                  thisRadioList.visible = true
                  thisRadioList.dataModel = result
                  thisIndexLettersView.dataModel = result
              } 
         })
         app.player.radio.downloadFinished.connect(function(playlist, result) {
             if(playlist == thisObject.playlistURL && result.length > 0) {
                 app.analytics.playRadio(playlist)
                 var flag = ""
                 if(thisObject.selectedRadio) {
                     flag = thisObject.selectedRadio.flag
                 }
                 app.player.playRadio(result[0], flag)
             }
         })
         addBuyButton()
     }
    attachedObjects: [
        ComponentDefinition {
            id: buyAppComponentDefinition
            source: "BuyActionItem.qml"
        },
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
    actions: [
        PlayerActionItem {
            ActionBar.placement: ActionBarPlacement.Signature
            navigationPane: liveStreamRadioPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        ImportSongsActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        OpenSongActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        EqualizerActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        SleepTimerActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        SettingsMenuAction {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
}
