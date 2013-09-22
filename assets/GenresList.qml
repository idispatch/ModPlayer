import bb.cascades 1.0

Page {
    id: genresPage
    objectName: "genresPage"
    property variant navigationPane
    titleBar: PlayerTitleBar {
        title: qsTr("Select Songs by Genre")
    }
    ViewContainer {
        ProgressComponent {
            id: progress
        }
        ListView {
            id: genresList
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
                        description: " "
                        middleStatus: qsTr("%1 songs").arg(ListItem.data.count)
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
    property int requestId
    function updateView(responseId, result) {
        if(responseId != requestId) 
            return
        progress.running = false
        genresList.visible = true
        genresList.dataModel = result
    }
    function load() {
        if(genresList.dataModel == null || genresList.dataModel.size() == 0) {
            progress.running = true
            genresList.visible = false
            requestId = app.player.catalog.findGenresAsync()
        }
    }
    function showPlayerView() {
        if(mainTabPane.activePane == navigationPane && navigationPane.top == genresPage) {
            var view = songPlayer.createObject()
            view.navigationPane = navigationPane
            navigationPane.push(view)
        }
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(showPlayerView)
        app.catalog.resultReady.connect(updateView)
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
            navigationPane: genresPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        OpenSongActionItem {
            navigationPane: genresPage.navigationPane
        },
        AppWorldActionItem{} 
    ]
}
