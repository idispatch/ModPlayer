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
                var view = songList.createObject()
                view.navigationPane = navigationPane
                view.loadSongsByFormat(chosenItem.id, chosenItem.name)
                navigationPane.push(view)
            }
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: songList
            source: "SongsList.qml"
        }
    ]
    
    actions: [ 
        ActionItem {
            title: "Player"
            imageSource: "asset:///images/icon_player.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            shortcuts: Shortcut {
                key: "p"
            } 
            onTriggered: {
                var view = songPlayer.createObject(parent)
                view.navigationPane = navigationPane
                navigationPane.push(view)
            }
            attachedObjects: [
                ComponentDefinition {
                    id: songPlayer
                    source: "SongPlayer.qml"
                }
            ]
        }
    ]
}
