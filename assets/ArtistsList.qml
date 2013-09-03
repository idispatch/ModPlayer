import bb.cascades 1.0

Page {
    id: artistsPage
    property variant navigationPane
    titleBar: PlayerTitleBar {
        title: qsTr("Select Songs by Artist")
    }
    ViewContainer {
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
                        upperStatus: qsTr("%1 songs").arg(ListItem.data.count)
                        middleStatus: qsTr("score %1 of 10").arg(ListItem.data.score)
                        lowerStatus: qsTr("rating %1").arg(ListItem.data.rating)
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
        if(!artistsList.dataModel) {
            artistsList.dataModel = app.player.catalog.findArtists()
        }
    }
    actions: [
        PlayerActionItem {
            navigationPane: artistsPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{} 
    ]
}
