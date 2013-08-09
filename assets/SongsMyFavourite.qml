import bb.cascades 1.0

Page {
    objectName: "SongsMyFavourite"
    
    property variant navigationPane
    
    titleBar: TitleBar {
        title: "My Favourite Songs"
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
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        
                        Container {
                            layout: StackLayout {
                                orientation: LayoutOrientation.TopToBottom
                            }
                            Label {
                                text: ListItemData.title
                                textStyle {
                                    fontSize: FontSize.Large
                                }
                            }
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                Label {
                                    text: ListItemData.fileName
                                    textStyle {
                                        fontSize: FontSize.Small
                                        color: Color.DarkGray
                                    }
                                }
                                ImageView {
                                    imageSource: "asset:///images/badge_played.png"
                                    verticalAlignment: VerticalAlignment.Center
                                }
                                Label {
                                    text: ListItemData.playCount
                                    textStyle {
                                        fontSize: FontSize.Small
                                        color: Color.DarkGray
                                    }
                                }
                            }
                            Divider{}
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
        songs.dataModel = app.player.catalog.findMyFavouriteSongs()
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
