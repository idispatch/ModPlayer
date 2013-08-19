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
                        middleStatus: "score: " + ListItemData.score + " of 10"
                        lowerStatus: "rating: " + ListItemData.rating
                    }
                }
            ]
            onCreationCompleted: {
                if(dataModel != null) {
                    var old = dataModel
                    dataModel = null
                    old.destroy()
                }
                dataModel = app.player.catalog.artists
            }
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songList.createObject()
                view.navigationPane = navigationPane
                view.loadSongsByArtist(chosenItem.id, chosenItem.name)
                navigationPane.push(view)
            }
            attachedObjects: [
                ComponentDefinition {
                    id: songList
                    source: "SongsList.qml"
                }
            ]
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
