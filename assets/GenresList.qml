import bb.cascades 1.1

Page {
    titleBar: TitleBar {
        title: qsTr("Select Songs by Genre")
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    Container {
        ListView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            dataModel: app.player.catalog.genres
            
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Header {
                        title: ListItemData
                    }
                },
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        title: ListItemData.name
                        status: ListItemData.id
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songs.createObject()
                view.loadSongsByGenre(chosenItem.id, chosenItem.name)
                view.navigationPane = songGenresNavigationPane
                songGenresNavigationPane.push(view)
            }
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: songs
            source: "SongsList.qml"
        }
    ]
}
