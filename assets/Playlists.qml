import bb.cascades 1.0

Page {
    id: playlistsPage
    objectName: "playlistsPage"
    property variant navigationPane
    property int requestId
    titleBar: PlayerTitleBar {
        title: qsTr("Select Playlist")
    }
    ViewContainer {
        ProgressComponent {
            id: progress
        }
        ListView {
            id: playlistsList
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
    function unload() {
        progress.start()
        playlistsList.visible = false
        if (playlistsList.dataModel) {
            playlistsList.dataModel.clear()
        }
        playlistsList.resetDataModel()
    }
    function load() {
        if(playlistsList.dataModel == null || playlistsList.dataModel.size() == 0) {
            progress.start()
            playlistsList.visible = false
            requestId = app.player.catalog.findPlaylistsAsync()
        }
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(function() {
                if(mainTabPane.activePane == navigationPane && 
                   navigationPane.top == playlistsPage) {
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
            playlistsList.visible = true
            playlistsList.dataModel = result
    })
    }
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
    actions: [
        CreatePlaylistActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        PlayerActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            navigationPane: playlistsPage.navigationPane
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
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        } 
    ]
}
