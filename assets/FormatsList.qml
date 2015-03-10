import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

Page {
    id: formatsPage
    objectName: "formatsPage"
    property variant navigationPane
    property int requestId
    titleBar: PlayerTitleBar {
        title: qsTr("Select Songs by Format") + Retranslate.onLanguageChanged
    }
    Container {
        layout: DockLayout {}
        WallpaperView {}
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
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
                        favourite: false
                        description: ListItem.data.description
                        middleStatus: qsTr("%1 songs").arg(ListItem.data.count) + Retranslate.onLanguageChanged
                        lowerStatus: Global.formatDuration(ListItem.data.duration)
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
        SleepTimerDisplay {}
        PlaylistControl {}
    }
    function load() {
        if(formatsList.dataModel == null || formatsList.dataModel.size() == 0) {
            progress.start()
            formatsList.visible = false
            requestId = app.player.catalog.findFormatsAsync()
        }
    }
    function reload() {
        progress.start()
        if(formatsList.dataModel) {
            formatsList.dataModel.clear()
        }
        formatsList.visible = false
        formatsList.resetDataModel()
        requestId = app.player.catalog.findFormatsAsync()
    }
    function addBuyButton() {
        if(!app.isExtendedVersion) {
            var buyActionItem = buyAppComponentDefinition.createObject()
            if(buyActionItem) {
                addAction(buyActionItem, ActionBarPlacement.InOverflow)
            }
        }
    }
    onCreationCompleted: {
        var thisObject = formatsPage
        app.player.requestPlayerView.connect(function() {
            if(mainTabPane.activePane == navigationPane && 
               navigationPane.top == formatsPage) {
                var view = songPlayer.createObject()
                if(view) {
                    view.navigationPane = navigationPane
                    navigationPane.push(view)
                }
            }
        })
        app.catalog.resultReady.connect(function(responseId, result) {
            if(responseId == thisObject.requestId) {
                thisObject.requestId = 0
                progress.stop()
                formatsList.visible = true
                formatsList.dataModel = result
            } 
        })
        addBuyButton()
    }
    attachedObjects: [
        ComponentDefinition {
            id: buyAppComponentDefinition
            source: "BuyActionItem.qml"
        },
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
        PlayModeActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        ImportSongsActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
            onCompleted: {
                reload()
            }
        },
        OpenSongActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        EqualizerActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        SleepTimerActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
}
