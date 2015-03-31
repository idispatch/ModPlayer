import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

Page {
    id: artistsPage
    property variant navigationPane
    property int requestId
    titleBar: PlayerTitleBar {
        id: titleBar
        title: qsTr("Select Artist") + Retranslate.onLanguageChanged
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            TitleBarText {
                title: titleBar.title
            }
            expandableArea {
                content: SearchArea {
                    id: searchArea
                    hintText: qsTr("search artists") + Retranslate.onLanguageChanged
                    onSearch: {
                        artistsPage.load()
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
                id: artistsList
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
                            upperStatus: ListItem.data.count > 0 ? qsTr("%1 songs").arg(ListItem.data.count) + Retranslate.onLanguageChanged : ""
                            middleStatus: ListItem.data.score > 0 ? qsTr("score %1 of 10").arg(ListItem.data.score) + Retranslate.onLanguageChanged : ""
                            lowerStatus: {
                                if(ListItem.data.rating > 0) {
                                    return qsTr("rating %1").arg(ListItem.data.rating) + Retranslate.onLanguageChanged
                                } else {
                                    return Global.formatDuration(ListItem.data.duration)
                                }
                            }
                        }
                    }
                ]
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath)
                    var view = songList.createObject()
                    if(view) {
                        view.navigationPane = navigationPane
                        navigationPane.push(view)
                        view.loadSongsByArtist(chosenItem.id, chosenItem.name)
                    }
                }
                attachedObjects: [
                    ComponentDefinition {
                        id: songList
                        source: "SongList.qml"
                    }
                ]
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
                artistsList.scrollToItem(indexPath, app.scrollAnimationType)
            }
        }
        SleepTimerDisplay {}
        PlaylistControl {}
    }
    function unload() {
        progress.start()
        if(artistsList.dataModel) {
            artistsList.dataModel.clear()
        }
        indexLettersView.dataModel = null
        artistsList.resetDataModel()
    }
    function load() {
        unload()
        requestId = app.catalog.findArtistsAsync(searchArea.searchTerm)
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
        var thisObject = artistsPage
        var thisSongPlayer = songPlayer
        app.player.requestPlayerView.connect(function() {
            if(mainTabPane.activePane == thisObject.navigationPane && 
               thisObject.navigationPane.top == thisObject) {
                var view = thisSongPlayer.createObject()
                if(view) {
                    view.navigationPane = thisNavigationPane
                    thisNavigationPane.push(view)
                }
            }
        })
        var thisArtistsList = artistsList
        var thisIndexLettersView = indexLettersView
        var thisProgress = progress
        app.catalog.resultReady.connect(function(responseId, result) {
            if(responseId == thisObject.requestId) { 
                thisObject.requestId = 0
                thisArtistsList.dataModel = result
                thisIndexLettersView.dataModel = result
                thisProgress.stop()
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
            ActionBar.placement: ActionBarPlacement.OnBar
            navigationPane: artistsPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        PlayModeActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        ImportSongsActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
            onCompleted: {
                load()
            }
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
