import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

Page {
    id: playlistsPage
    property variant navigationPane
    property int requestId
    titleBar: PlayerTitleBar {
        id: titleBar
        title: qsTr("Select Playlist") + Retranslate.onLanguageChanged
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            TitleBarText {
                title: titleBar.title
            }
            expandableArea {
                content: SearchArea {
                    id: searchArea
                    hintText: qsTr("search playlists") + Retranslate.onLanguageChanged
                    onSearch: {
                        playlistsPage.load()
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
        ProgressComponent {
            id: progress
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            ListView {
                id: playlistsList
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                topPadding: 20
                bottomPadding: 20
                leftPadding: 10
                rightPadding: 10
                function playPlaylist(playlist) {
                    var songs = app.catalog.getPlaylistSongs(playlist.id)
                    if(songs.length > 0) {
                        app.player.playlist.assign(songs)
                        app.player.playPlaylist()
                    }
                }
                function deletePlaylist(playlist) {
                    app.catalog.deletePlaylistById(playlist.id)
                    unload()
                    load()
                }
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
                            id: playlistEntry
                            implicitLayoutAnimationsEnabled: false
                            title: ListItem.data.name
                            description: " "
                            middleStatus: qsTr("%1 songs").arg(ListItem.data.count) + Retranslate.onLanguageChanged
                            lowerStatus: Global.formatDuration(ListItem.data.duration)
                            imageSource: "asset:///images/formats/icon_playlist.png"
                            contextActions: [
                                ActionSet {
                                    title: playlistEntry.ListItem.data.name
                                    subtitle: qsTr("Playlist %1").arg(playlistEntry.ListItem.data.name) + Retranslate.onLanguageChanged
                                    actions: [
                                        ActionItem {
                                            title: qsTr("Play Playlist") + Retranslate.onLanguageChanged
                                            enabled: playlistEntry.ListItem.data.count > 0
                                            imageSource: "asset:///images/actions/icon_play.png"
                                            onTriggered: {
                                                playlistEntry.ListItem.view.playPlaylist(playlistEntry.ListItem.data)
                                            }
                                        },
                                        DeleteActionItem {
                                            title: qsTr("Delete Playlist") + Retranslate.onLanguageChanged
                                            onTriggered: {
                                                playlistEntry.ListItem.view.deletePlaylist(playlistEntry.ListItem.data)
                                            }
                                        }
                                    ]
                                }
                            ]
                            function viewInitialized(initialized) {
                                if(initialized) {
                                    scaleX = 0
                                    scaleY = 0
                                    listItemAnimation.play()
                                }
                            }
                            onCreationCompleted: {
                                ListItem.initializedChanged.connect(viewInitialized)
                            }
                            animations: [
                                ParallelAnimation {
                                    id: listItemAnimation
                                    FadeTransition {
                                        fromOpacity: 0.0
                                        toOpacity: 1.0
                                        duration: 600
                                        easingCurve: StockCurve.Linear
                                    }
                                    ScaleTransition {
                                        toX: 1.0
                                        toY: 1.0
                                        fromX: 0.0
                                        fromY: 0.0
                                        duration: 600
                                        easingCurve: StockCurve.CubicInOut
                                    }
                                }
                            ]
                        }
                    }
                ]
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath)
                    var view = songList.createObject()
                    view.navigationPane = navigationPane
                    navigationPane.push(view)
                    view.loadSongsByPlaylist(chosenItem.id, chosenItem.name)
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
                playlistsList.scrollToItem(indexPath, app.scrollAnimationType)
            }
        }
        SleepTimerDisplay {}
        PlaylistControl {}
    }
    function unload() {
        progress.start()
        if(playlistsList.dataModel) {
            playlistsList.dataModel.clear()
        }
        indexLettersView.dataModel = null
        playlistsList.resetDataModel()
    }
    function load() {
        unload()
        requestId = app.player.catalog.findPlaylistsAsync(searchArea.searchTerm)
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
        var thisObject = playlistsPage
        app.player.requestPlayerView.connect(function() {
            if(thisMainTabPane.activePane == thisObject.navigationPane && thisObject.navigationPane.top == thisObject) {
                thisObject.showPlayer()
            }
        })
        var thisProgress = progress
        var thisPlaylistsList = playlistsList
        var thisIndexLettersView = indexLettersView
        app.catalog.resultReady.connect(function(responseId, result) {
            if(responseId != thisObject.requestId) 
                return
            thisObject.requestId = 0
            thisProgress.stop()
            thisPlaylistsList.visible = true
            thisPlaylistsList.dataModel = result
            thisIndexLettersView.dataModel = result
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
        CreatePlaylistActionItem {
            ActionBar.placement: ActionBarPlacement.Signature
            onPlaylistCreated: {
                unload()
                load()
            }
        },
        PlayerActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            navigationPane: playlistsPage.navigationPane
        },
        DeleteAllPlaylistsActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            enabled: playlistsList.dataModel != null && playlistsList.dataModel.size() > 0 
            onPlaylistsDeleted: {
                unload()
                load()
            }
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
