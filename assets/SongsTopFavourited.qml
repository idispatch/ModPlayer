import bb.cascades 1.1

Page {
    property variant navigationPane
    
    objectName: "SongsTopFavourited" 
    
    titleBar: TitleBar {
        title: "Top Favourited Songs"
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    
    Container {
        ListView {
            id: songs
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            listItemComponents: [
                ListItemComponent {
                    StandardListItem {
                        title: ListItemData.title
                        description: ListItemData.fileName
                        status: ListItemData.downloads
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songView.createObject()
                view.navigationPane = navigationPane
                view.load(chosenItem.id)
                navigationPane.push(view)
            }
        }
    }
    
    function load() {
        songs.dataModel = app.player.catalog.findMostFavouritedSongs()
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: songView
            source: "SongView.qml"
        }
    ]
}

