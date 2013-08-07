import bb.cascades 1.1

Page {
    property variant navigationPane
    
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
                var view = songList.createObject()
                view.navigationPane = navigationPane
                view.loadSongsByGenre(chosenItem.id, chosenItem.name)
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
                var view = songPlayer.createObject()
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
