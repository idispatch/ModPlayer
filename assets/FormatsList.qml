import bb.cascades 1.1

Page {
    property variant navigationPane
    
    titleBar: TitleBar {
        title: "Select Songs by Format"
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    
    Container {
        ListView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            dataModel: app.player.catalog.formats
            
            listItemComponents: ListItemComponent {
                StandardListItem {
                    title: ListItemData.name
                    description: ListItemData.description
                    status: ListItemData.count
                }
            }
        
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songsList.createObject()
                view.navigationPane = navigationPane
                view.loadSongsByFormat(chosenItem.id, chosenItem.name)
                navigationPane.push(view)
            }
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: songsList
            source: "SongsList.qml"
        }
    ]
}
