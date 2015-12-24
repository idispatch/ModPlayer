import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

Page {
    id: albumsPage
    property variant navigationPane
    property int requestId
    titleBar: PlayerTitleBar {
        id: titleBar
        title: qsTr("Select Album") + Retranslate.onLanguageChanged
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            TitleBarText {
                title: titleBar.title
            }
            expandableArea {
                content: SearchArea {
                    id: searchArea
                    hintText: qsTr("search albums") + Retranslate.onLanguageChanged
                    onSearch: {
                        albumsPage.load()
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
                id: albumsList
                visible: !progress.running
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                topPadding: 20
                bottomPadding: topPadding
                leftPadding: 10
                rightPadding: leftPadding
                function playAlbum(album) {
                    var songs = app.catalog.getAlbumSongs(album.id)
                    app.player.playlist.assign(songs)
                    if(app.player.playlist.count > 0) {
                        app.player.playPlaylist()
                    }
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
                            id: albumEntry
                            title: ListItem.data.name
                            description: ListItem.data.artistName
                            middleStatus: qsTr("%1 songs").arg(ListItem.data.count) + Retranslate.onLanguageChanged
                            lowerStatus: Global.formatDuration(ListItem.data.duration)
                            imageSource: "asset:///images/formats/icon_album.png"
                            contextActions: [
                                ActionSet {
                                    title: albumEntry.ListItem.data.name
                                    subtitle: qsTr("Album %1").arg(albumEntry.ListItem.data.name) + Retranslate.onLanguageChanged
                                    actions: [
                                        ActionItem {
                                            title: qsTr("Play Album") + Retranslate.onLanguageChanged
                                            enabled: albumEntry.ListItem.data.count > 0
                                            imageSource: "asset:///images/actions/icon_play.png"
                                            onTriggered: {
                                                albumEntry.ListItem.view.playAlbum(albumEntry.ListItem.data)
                                            }
                                        }
                                    ]
                                }
                            ]
                            implicitLayoutAnimationsEnabled: false
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
                    if(view) {
                        view.navigationPane = navigationPane
                        navigationPane.push(view)
                        view.loadSongsByAlbum(chosenItem.id, chosenItem.name)
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
                albumsList.scrollToItem(indexPath, app.scrollAnimationType)
            }
        }
        SleepTimerDisplay {}
        PlaylistControl {
            navigationPane: albumsPage.navigationPane
        }
    }
    function unload() {
        progress.start()
        if(albumsList.dataModel) {
            albumsList.dataModel.clear()
        }
        indexLettersView.dataModel = null
        albumsList.resetDataModel()
    }
    function load() {
        unload()
        requestId = app.player.catalog.findAlbumsAsync(searchArea.searchTerm)
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
        var thisObject = albumsPage
        app.player.requestPlayerView.connect(function() {
            if(thisMainTabPane.activePane == thisObject.navigationPane && thisObject.navigationPane.top == thisObject) {
                thisObject.showPlayer()
            }
        })
        var thisAlbumsList = albumsList
        var thisIndexLettersView = indexLettersView
        var thisProgress = progress
        app.catalog.resultReady.connect(function(responseId, result) {
            if(responseId == thisObject.requestId) { 
                thisObject.requestId = 0
                thisAlbumsList.dataModel = result
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
            navigationPane: albumsPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.Signature
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
