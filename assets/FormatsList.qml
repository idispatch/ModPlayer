import bb.cascades 1.0

Page {
    id: formatsPage
    property variant navigationPane
    titleBar: PlayerTitleBar {
        title: qsTr("Select Songs by Format")
    }
    ViewContainer {
        ListView {
            id: formatsList
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            topPadding: 20
            bottomPadding: topPadding
            leftPadding: 10
            rightPadding: leftPadding
            listItemComponents: ListItemComponent {
                ModPlayerListItem {
                    title: ListItem.data.name
                    description: ListItem.data.description
                    middleStatus: qsTr("%1 songs").arg(ListItem.data.count)
                    imageSource: ListItem.data.iconPath
                }
            }
            function load() {
                if(!dataModel) {
                    dataModel = app.player.catalog.findFormats()
                }
            }
            onCreationCompleted: load()
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songList.createObject()
                view.navigationPane = navigationPane
                navigationPane.push(view)
                view.loadSongsByFormat(chosenItem.id, chosenItem.name)
            }
            attachedObjects: [
                ComponentDefinition {
                    id: songList
                    source: "SongList.qml"
                }
            ]
        }
    }
    function load() {
        formatsList.load()
    }
    actions: [
        PlayerActionItem {
            navigationPane: formatsPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{} 
    ]
}
