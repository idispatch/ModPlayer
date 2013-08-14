import bb.cascades 1.0

Page {
    property variant navigationPane
    
    titleBar: TitleBar {
        title: qsTr("Select Songs by Format")
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    
    Container {
        background: back.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.Fill
                imageSource: "asset:///images/backgrounds/background.png"
            }
        ]
        ListView {
            property variant app_ref: app
            
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            listItemComponents: ListItemComponent {
                StandardListItem {
                    title: ListItemData.name
                    description: ListItemData.description
                    status: ListItemData.count + " songs"
                    imageSource: ListItem.view.app_ref.getIconPath(ListItemData)
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
