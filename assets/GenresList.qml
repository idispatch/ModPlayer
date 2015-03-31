import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

Page {
    id: genresPage
    property variant navigationPane
    property int requestId
    titleBar: PlayerTitleBar {
        id: titleBar
        title: qsTr("Select Songs by Genre") + Retranslate.onLanguageChanged
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            TitleBarText {
                title: titleBar.title
            }
            expandableArea {
                content: SearchArea {
                    id: searchArea
                    hintText: qsTr("search genres") + Retranslate.onLanguageChanged
                    onSearch: {
                        genresPage.load()
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
                id: genresList
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
                            description: " "
                            middleStatus: qsTr("%1 songs").arg(ListItem.data.count) + Retranslate.onLanguageChanged
                            lowerStatus: Global.formatDuration(ListItem.data.duration)
                        }
                    }
                ]
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath)
                    var view = songList.createObject()
                    view.navigationPane = navigationPane
                    navigationPane.push(view)
                    view.loadSongsByGenre(chosenItem.id, chosenItem.name)
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
                genresList.scrollToItem(indexPath, app.scrollAnimationType)
            }
        }
        SleepTimerDisplay {}
        PlaylistControl {}
    }
    function unload() {
        progress.start()
        if(genresList.dataModel) {
            genresList.dataModel.clear()
        }
        indexLettersView.dataModel = null
        genresList.resetDataModel()
    }
    function load() {
        unload()
        requestId = app.player.catalog.findGenresAsync(searchArea.searchTerm)
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
        var thisObject = genresPage
        app.player.requestPlayerView.connect(function() {
            if(thisMainTabPane.activePane == thisObject.navigationPane && thisObject.navigationPane.top == thisObject) {
                thisObject.showPlayer()
            }
        })
        var thisProgress = progress
        var thisGenresList = genresList
        var thisIndexLettersView = indexLettersView
        app.catalog.resultReady.connect(function(responseId, result) {
            if(responseId == thisObject.requestId) {
                thisObject.requestId = 0
                thisProgress.stop()
                thisGenresList.visible = true
                thisGenresList.dataModel = result
                thisIndexLettersView.dataModel = result
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
            navigationPane: genresPage.navigationPane
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
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        } 
    ]
}
