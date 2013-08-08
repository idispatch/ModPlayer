import bb.cascades 1.0

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
                var view = songList.createObject()
                view.navigationPane = navigationPane
                view.loadSongsByFormat(chosenItem.id, chosenItem.name)
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
            navigationPane: parent.navigationPane
        } 
    ]
}
