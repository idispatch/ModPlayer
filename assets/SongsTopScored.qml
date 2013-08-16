import bb.cascades 1.0

Page {
    objectName: "SongsTopScored"

    property variant navigationPane

    titleBar: TitleBar {
        title: qsTr("Top Scored Songs")
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
                imageSource: "asset:///images/backgrounds/background.png"
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
                        title: ListItem.data.title
                        description: ListItem.data.fileName
                        status: ListItem.data.score + " of 10"
                        imageSource: ListItem.data.iconPath
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

