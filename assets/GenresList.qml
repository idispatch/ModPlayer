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
            id: genresList
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
                        middleStatus: qsTr("%1 songs").arg(ListItemData.count)
                    }
                }
            ]
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songList.createObject()
                view.navigationPane = navigationPane
                navigationPane.push(view)
                view.loadSongsByGenre(chosenItem.id, chosenItem.name)
            }
            attachedObjects: [
                ComponentDefinition {
                    id: songList
                    source: "SongsList.qml"
                }
            ]
        }
    }
    function load() {
        if(!genresList.dataModel) {
            genresList.dataModel = app.player.catalog.findGenres()
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
