import bb.cascades 1.0

Page {
    property variant navigationPane
    
    titleBar: TitleBar {
        title: "Select Songs by Format"
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    
    Container {
        background: back.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.Fill
                imageSource: "asset:///images/background.png"
            }
        ]
        ListView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            listItemComponents: ListItemComponent {
                StandardListItem {
                    title: ListItemData.name
                    description: ListItemData.description
                    status: ListItemData.count + " songs"
                    imageSource: "asset:///images/icon_" + ListItemData.name.toLowerCase() + ".png"
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
            onCreationCompleted: {
                dataModel = app.player.catalog.formats
            }
        }
    }
    
    actions: [
        PlayerActionItem {
            navigationPane: parent.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        } 
    ]
}
