import bb.cascades 1.1

Page {
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
                    status: ListItemData.id
                }
            }
        
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songsByFormatView.createObject()
                view.formatId = chosenItem.id
                view.formatName = chosenItem.name
                songFormatsNavigationPane.push(view)
            }
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: songsByFormatView
            source: "SongsByFormat.qml"
        }
    ]
}
