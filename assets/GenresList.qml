import bb.cascades 1.0

Page {
    id: genresPage
    property variant navigationPane
    
    titleBar: TitleBar {
        title: qsTr("Select Songs by Genre")
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    
    ViewContainer {
        ListView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            topPadding: 20
            bottomPadding: topPadding
            leftPadding: 10
            rightPadding: leftPadding
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Label {
                        text: ListItem.data
                        textStyle.fontWeight: FontWeight.Bold
                        textStyle.fontSize: FontSize.Large
                        textStyle.color: Color.White
                    }
                },
                ListItemComponent {
                    type: "item"
                    ModPlayerListItem {
                        title: ListItemData.name
                        description: " "
                        middleStatus: ListItemData.count + " songs"
                    }
                }
            ]
            onCreationCompleted: {
                dataModel = app.player.catalog.genres
            }
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songList.createObject()
                view.navigationPane = navigationPane
                view.loadSongsByGenre(chosenItem.id, chosenItem.name)
                navigationPane.push(view)
            }
            attachedObjects: [
                ComponentDefinition {
                    id: songList
                    source: "SongsList.qml"
                }
            ]
        }
    }
    actions: [
        PlayerActionItem {
            navigationPane: genresPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        } 
    ]
}
