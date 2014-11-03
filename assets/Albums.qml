import bb.cascades 1.3
import "functions.js" as Global

Page {
    id: albumsPage
    objectName: "albumsPage"
    property variant navigationPane
    property int requestId
    titleBar: PlayerTitleBar {
        id: titleBar
        title: qsTr("Select Album")
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            HorizontalContainer {
                leftPadding: 10
                Label {
                    text: titleBar.title
                    textStyle {
                        color: Color.White 
                        fontSize: FontSize.Large
                    }
                    verticalAlignment: VerticalAlignment.Center
                }
            }
            expandableArea {
                content: SearchArea {
                    id: searchArea
                    hintText: qsTr("search albums")
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
    ViewContainer {
        ProgressComponent {
            id: progress
        }
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
                        middleStatus: qsTr("%1 songs").arg(ListItem.data.count)
                        lowerStatus: Global.formatDuration(ListItem.data.duration)
                        imageSource: "asset:///images/formats/icon_album.png"
                        favourite: false
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
                                            albumEntry.ListItem.view.playAlbum(albumEntry.ListItem.data)
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
        if(albumsList.dataModel) {
            albumsList.dataModel.clear()
        }
        albumsList.resetDataModel()
    }
    function load() {
        unload()
        requestId = app.player.catalog.findAlbumsAsync(searchArea.searchTerm)
    }
    function showPlayer() {
        var view = songPlayer.createObject()
        view.navigationPane = navigationPane
        navigationPane.push(view)
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(function() {
             if(mainTabPane.activePane == navigationPane && 
                navigationPane.top == albumsPage) {
                 showPlayer()
             }
        })
        app.catalog.resultReady.connect(function(responseId, result) {
            if(responseId != requestId) 
                return
            requestId = 0
            albumsList.dataModel = result
            progress.stop()
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
            navigationPane: albumsPage.navigationPane
        },
        PlayModeActionItem {
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
