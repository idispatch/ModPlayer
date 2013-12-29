import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: artistsPage
    objectName: "artistsPage"
    property variant navigationPane
    property int requestId
    titleBar: PlayerTitleBar {
        title: qsTr("Select Songs by Artist")
    }
    ViewContainer {
        ProgressComponent {
            id: progress
        }
        ListView {
            id: artistsList
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
                        upperStatus: ListItem.data.count > 0 ? qsTr("%1 songs").arg(ListItem.data.count) : ""
                        middleStatus: ListItem.data.score > 0 ? qsTr("score %1 of 10").arg(ListItem.data.score) : ""
                        lowerStatus: {
                            if(ListItem.data.rating > 0) {
                                return qsTr("rating %1").arg(ListItem.data.rating)
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
                view.navigationPane = navigationPane
                navigationPane.push(view)
                view.loadSongsByArtist(chosenItem.id, chosenItem.name)
            }
            attachedObjects: [
                ComponentDefinition {
                    id: songList
                    source: "SongList.qml"
                }
            ]
        }
    }
    function load() {
        if(artistsList.dataModel == null || artistsList.dataModel.size() == 0) {
            progress.start()
            artistsList.visible = false
            requestId = app.catalog.findArtistsAsync()
        }
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(function() {
            if(mainTabPane.activePane == navigationPane && 
               navigationPane.top == artistsPage) {
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
            artistsList.visible = true
            artistsList.dataModel = result
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
            navigationPane: artistsPage.navigationPane
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
