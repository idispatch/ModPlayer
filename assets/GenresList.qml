import bb.cascades 1.1

Page {
    titleBar: TitleBar {
        title: "Select Songs by Genre"
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    Container {
        id: root
        ListView {
            id: listView
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
                console.debug(indexPath)
                var chosenItem = dataModel.data(indexPath)
                console.debug(chosenItem.name)
            }
        }
    }
}
