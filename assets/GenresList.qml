import bb.cascades 1.0

Page {
    id: genresPage
    property variant navigationPane
    titleBar: PlayerTitleBar {
        title: qsTr("Select Songs by Genre")
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
                        textStyle {
                            fontWeight: FontWeight.Bold
                            fontSize: FontSize.Large
                            color: Color.White
                        }
                    }
                },
                ListItemComponent {
                    type: "item"
                    ModPlayerListItem {
                        title: ListItem.data.name
                        description: " "
                        middleStatus: qsTr("%1 songs").arg(ListItem.data.count)
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
                    source: "PersonalSongList.qml"
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
        },
        AppWorldActionItem{} 
    ]
}
