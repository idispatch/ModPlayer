import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Page {
    id: artistsPage
    objectName: "artistsPage"
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
                        load()
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
        SleepTimerDisplay {}
        PlaylistControl {}
    }
    function unload() {
        progress.start()
        if(artistsList.dataModel) {
            artistsList.dataModel.clear()
        }
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
        app.player.requestPlayerView.connect(function() {
            if(mainTabPane.activePane == navigationPane && 
               navigationPane.top == artistsPage) {
                var view = songPlayer.createObject()
                if(view) {
                    view.navigationPane = navigationPane
                    navigationPane.push(view)
                }
            }
        })
        app.catalog.resultReady.connect(function(responseId, result) {
            if(responseId != requestId) 
                return
            requestId = 0
            artistsList.dataModel = result
            progress.stop()
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
