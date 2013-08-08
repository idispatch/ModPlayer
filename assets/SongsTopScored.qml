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
        }
    }
    
    function load() {
        if(songs.dataModel == null) {
            songs.dataModel = app.player.catalog.findMostScoredSongs()
        }
        progress.running = false
        progress.visible = false
        songs.visible = true
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: songView
            source: "SongView.qml"
        }
    ]
    
    actions: [ 
        PlayerActionItem {
            navigationPane: parent.navigationPane
        }
    ]
}

