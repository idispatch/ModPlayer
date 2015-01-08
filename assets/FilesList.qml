import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Page {
    id: filesPage
    objectName: "filesPage"
    property variant navigationPane
    property int requestId
    titleBar: PlayerTitleBar {
        id: titleBar
        title: qsTr("Select Songs") + Retranslate.onLanguageChanged
    }
    Container {
        layout: DockLayout {}
        WallpaperView {}
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            ListView {
                id: filesList
                visible: app.isExtendedVersion
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                topPadding: 20
                bottomPadding: topPadding
                leftPadding: 10
                rightPadding: leftPadding
                listItemComponents: [
                    ListItemComponent {
                        type: ""
                        Label {
                            text: ListItem.data.fileName
                            textStyle {
                                fontWeight: FontWeight.Bold
                                fontSize: FontSize.Large
                                color: Color.White
                            }
                        }
                    }
                ]
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath)
                    if(chosenItem.isDirectory) {
                        var view = songList.createObject()
                        if(view) {
                            view.loadPath(chosenItem.filePath)
                            view.navigationPane = navigationPane
                            navigationPane.push(view)
                        }
                    }
                }
                attachedObjects: [
                    ComponentDefinition {
                        id: songList
                        source: "FilesList.qml"
                    }
                ]
            }
        }
        SleepTimerDisplay {}
        PlaylistControl {}
    }
    function unload() {
        if(filesList.dataModel) {
            filesList.dataModel.clear()
        }
        filesList.resetDataModel()
    }
    function load() {
        unload()
        filesList.dataModel = app.fileSystem.listFiles("/")
    }
    function loadPath(path) {
        filesList.dataModel = app.fileSystem.listFiles(path)
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(function() {
             if(mainTabPane.activePane == navigationPane && 
                 navigationPane.top == filesPage) {
                 var view = songPlayer.createObject()
                 view.navigationPane = navigationPane
                 navigationPane.push(view)
             }
        })
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
            navigationPane: filesPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        PlayModeActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        ImportSongsActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        OpenSongActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        SleepTimerActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        BuyActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        } 
    ]
}
