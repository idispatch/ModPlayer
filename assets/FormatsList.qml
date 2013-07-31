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
                    status: ListItemData.count
                }
            }
        
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songsList.createObject()
                view.navigationPane = songFormatsNavigationPane
                view.loadSongsByFormat(chosenItem.id, chosenItem.name)
                songFormatsNavigationPane.push(view)
                
                console.debug("34274: " + app.player.catalog.resolveFileNameById(34274))
                console.debug("34274: " + app.player.catalog.resolveModuleById(34274))
                console.debug("111.mod: " + app.player.catalog.resolveModuleByFileName('111.mod'))
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
