import bb.cascades 1.0

Page {
    objectName: "SongsTopScored"
    
    property variant navigationPane
    
    titleBar: TitleBar {
        title: "Top Scored Songs"
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    
    Container {
        layout: StackLayout {
        }
        background: back.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.Fill
                imageSource: "asset:///images/background.png"
            }
        ]
        ProgressComponent {
            id: progress
        }
        ListView {
            id: songs
            visible: false
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            listItemComponents: [
                ListItemComponent {
                    StandardListItem {
                        title: ListItemData.title
                        description: ListItemData.fileName
                        status: ListItemData.score
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songView.createObject()
                view.navigationPane = navigationPane
                view.load(chosenItem.modId)
                navigationPane.push(view)
            }
            
            attachedObjects: [
                ComponentDefinition {
                    id: songView
                    source: "SongView.qml"
                }
            ]
        }
    }
    
    function load() {
        songs.dataModel = app.player.catalog.findMostScoredSongs()
        progress.running = false
        progress.visible = false
        songs.visible = true
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

