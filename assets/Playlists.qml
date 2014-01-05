import bb.cascades 1.0
import "functions.js" as Global

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
            function playPlaylist(playlist) {
                var songs = app.catalog.getPlaylistSongs(playlist.id)
                if(songs.length > 0) {
                    app.player.playlist.clear()
                    for(var i = 0; i < songs.length; ++i) {
                        app.player.playlist.add(songs[i])
                    }
                }
                if(app.player.playlist.count > 0) {
                    app.player.playPlaylist()
                    showPlayer()
                }
            }
            function deletePlaylist(playlist) {
                app.catalog.deletePlaylist(playlist.id)
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
                        title: ListItem.data.name
                        description: " "
                        middleStatus: qsTr("%1 songs").arg(ListItem.data.count)
                        lowerStatus: Global.formatDuration(ListItem.data.duration)
                        imageSource: "asset:///images/formats/icon_playlist.png"
                        contextActions: [
                            ActionSet {
                                title: playlistEntry.ListItem.data.name
                                subtitle: qsTr("Playlist %1").arg(playlistEntry.ListItem.data.name)
                                actions: [
                                    ActionItem {
                                        title: qsTr("Play Playlist")
                                        enabled: playlistEntry.ListItem.data.count > 0
                                        imageSource: "asset:///images/actions/icon_play.png"
                                        onTriggered: {
                                            playlistEntry.ListItem.view.playPlaylist(playlistEntry.ListItem.data)
                                        }
                                    },
                                    DeleteActionItem {
                                        title: qsTr("Delete Playlist")
                                        onTriggered: {
                                            playlistEntry.ListItem.view.deletePlaylist(playlistEntry.ListItem.data)
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
        if(playlistsList.dataModel) {
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
            navigationPane: playlistsPage.navigationPane
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
