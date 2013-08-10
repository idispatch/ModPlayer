import bb.cascades 1.0

Page {
    objectName: "SongsTopDownloaded"

    property variant navigationPane

    titleBar: TitleBar {
        title: "Top Downloaded Songs"
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
                        status: ListItemData.downloads + " downloads"
                        imageSource: "asset:///images/icon_" + formatToIcon[ListItemData.formatId] + ".png"
                        property variant formatToIcon: {
                            1: "mod",
                            2: "669",
                            3: "it",
                            4: "med",
                            5: "mtm",
                            6: "oct",
                            7: "okt",
                            8: "s3m",
                            9: "stm",
                            10: "xm"
                        }
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
        songs.dataModel = app.player.catalog.findMostDownloadedSongs()
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

