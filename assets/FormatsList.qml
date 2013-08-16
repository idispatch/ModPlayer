import bb.cascades 1.0

Page {
    id: formatsPage
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
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            listItemComponents: ListItemComponent {
                StandardListItem {
                    title: ListItem.data.name
                    description: ListItem.data.description
                    status: ListItem.data.count + " songs"
                    imageSource: ListItem.data.iconPath
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
            navigationPane: formatsPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        } 
    ]
}
