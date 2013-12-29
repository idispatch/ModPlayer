import bb.cascades 1.0

Page {
    id: albumsPage
    objectName: "albumsPage"
    property variant navigationPane
    property int requestId
    titleBar: PlayerTitleBar {
        title: qsTr("Select Album")
    }
    ViewContainer {
        ProgressComponent {
            id: progress
        }
        ListView {
            id: albumsList
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
                        id: albumEntry
                        title: ListItem.data.name
                        description: " "
                        middleStatus: qsTr("%1 songs").arg(ListItem.data.count)
                        imageSource: "asset:///images/formats/icon_album.png"
                        contextActions: [
                            ActionSet {
                                title: albumEntry.ListItem.data.name
                                subtitle: qsTr("Album %1").arg(albumEntry.ListItem.data.name)
                                actions: [
                                    ActionItem {
                                        title: qsTr("Play Album")
                                        enabled: albumEntry.ListItem.data.count > 0
                                        imageSource: "asset:///images/actions/icon_play.png"
                                        onTriggered: {
                                            //albumEntry.ListItem.view.playPlaylist(playlistEntry.ListItem.data)
                                        }
                                    }
                                ]
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
                view.loadSongsByAlbum(chosenItem.id, chosenItem.name)
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
        albumsList.visible = false
        if(albumsList.dataModel) {
            albumsList.dataModel.clear()
        }
        albumsList.resetDataModel()
    }
    function load() {
        if(albumsList.dataModel == null || albumsList.dataModel.size() == 0) {
            progress.start()
            albumsList.visible = false
            requestId = app.player.catalog.findAlbumsAsync()
        }
    }
    function showPlayer() {
        var view = songPlayer.createObject()
        view.navigationPane = navigationPane
        navigationPane.push(view)
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(function() {
                if(mainTabPane.activePane == navigationPane && 
                   navigationPane.top == playlistsPage) {
                    showPlayer()
                }
        })
    app.catalog.resultReady.connect(function(responseId, result) {
            if(responseId != requestId) 
                return
            requestId = 0
            progress.stop()
            albumsList.visible = true
            albumsList.dataModel = result
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
            navigationPane: albumsPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
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
