import bb.cascades 1.0

Page {
    id: formatsPage
    property variant navigationPane
    titleBar: TitleBar {
        title: qsTr("Select Songs by Format")
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
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
                    middleStatus: ListItem.data.count + " songs"
                    imageSource: ListItem.data.iconPath
                }
            }
            function load() {
                if(!dataModel) {
                    dataModel = app.player.catalog.formats
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
                    source: "SongsList.qml"
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
        } 
    ]
}
