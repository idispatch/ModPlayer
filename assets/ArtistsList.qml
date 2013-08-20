import bb.cascades 1.0

Page {
    id: artistsPage
    property variant navigationPane
    titleBar: TitleBar {
        title: qsTr("Select Songs by Artist")
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
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
                        textStyle.fontWeight: FontWeight.Bold
                        textStyle.fontSize: FontSize.Large
                        textStyle.color: Color.White
                    }
                },
                ListItemComponent {
                    type: "item"
                    ModPlayerListItem {
                        title: ListItemData.name
                        upperStatus: ListItemData.count + " songs"
                        middleStatus: "score " + ListItemData.score + " of 10"
                        lowerStatus: "rating " + ListItemData.rating
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
                    source: "SongsList.qml"
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
        } 
    ]
}
