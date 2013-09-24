import bb.cascades 1.0

Page {
    id: formatsPage
    objectName: "formatsPage"
    property variant navigationPane
    titleBar: PlayerTitleBar {
        title: qsTr("Select Songs by Format")
    }
    ViewContainer {
        ProgressComponent {
            id: progress
        }
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
    property int requestId
    function updateView(responseId, result) {
        if(responseId != requestId) 
            return
        progress.running = false
        formatsList.visible = true
        formatsList.dataModel = result
    }
    function load() {
        if(formatsList.dataModel == null || formatsList.dataModel.size() == 0) {
            progress.running = true
            formatsList.visible = false
            requestId = app.player.catalog.findFormatsAsync()
        }
    }
    function showPlayerView() {
        if(mainTabPane.activePane == navigationPane && navigationPane.top == formatsPage) {
            var view = songPlayer.createObject()
            view.navigationPane = navigationPane
            navigationPane.push(view)
        }
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(showPlayerView)
        app.catalog.resultReady.connect(updateView)
    }
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
    actions: [
        PlayerActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            navigationPane: formatsPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        OpenSongActionItem {
        },
        AppWorldActionItem{} 
    ]
}
