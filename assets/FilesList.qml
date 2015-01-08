import bb.cascades 1.0
import bb.system 1.0
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
        GroupContainer {
            visible: !app.isExtendedVersion
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            Label {
                text: qsTr("This feature is available in ModPlayer Plus.") + Retranslate.onLanguageChanged
                horizontalAlignment: HorizontalAlignment.Center
                multiline: true
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    fontWeight: FontWeight.Bold
                    fontSize: FontSize.Medium
                    color: Color.White 
                }
            }
        }
        Container {
            visible: app.isExtendedVersion
            topPadding: 20
            bottomPadding: 20
            leftPadding: 20
            rightPadding: 20
            GroupContainer {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                layout: DockLayout {}
                leftPadding: 0
                rightPadding: 0
                topMargin: 0
                bottomMargin: 0
                topPadding: 0
                bottomPadding: 4
                ListView {
                    id: filesList
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    topPadding: 20
                    bottomPadding: topPadding
                    leftPadding: 10
                    rightPadding: leftPadding
                    listItemComponents: [
                        ListItemComponent {
                            FileEntryView {
                                itemIndex: ListItem.indexInSection
                                fileName: ListItem.data.fileName
                                fileSize: ListItem.data.fileSize
                                fileType: ListItem.data.fileType
                                fileIcon: ListItem.data.fileIcon
                                isDirectory: ListItem.data.isDirectory
                            }
                        }
                    ]
                    onTriggered: {
                        var chosenItem = dataModel.data(indexPath)
                        if(chosenItem.isDirectory) {
                            var view = songList.createObject()
                            if(view) {
                                activityIndicator.view = view
                                activityIndicator.filePath = chosenItem.filePath
                                activityIndicator.visible = true
                                activityIndicator.start()
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
                Container {
                    visible: (!filesList.dataModel || filesList.dataModel.size() == 0)
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Center
                    Label {
                        text: qsTr("No songs in this directory") + Retranslate.onLanguageChanged
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        multiline: true
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText
                            fontWeight: FontWeight.Bold
                            fontSize: FontSize.Medium
                            color: Color.Black
                        }
                    }
                }
            }
        }
        ActivityIndicator {
            id: activityIndicator
            property variant view
            property string filePath
            running: false
            visible: false
            preferredHeight: 128
            preferredWidth: 128
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            onStarted: {
                activityIndicator.view.loadPath(activityIndicator.filePath)
                activityIndicator.stop()
                activityIndicator.visible = false
                activityIndicator.view.navigationPane = filesPage.navigationPane
                filesPage.navigationPane.push(activityIndicator.view)
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
                 if(view) {
                    view.navigationPane = navigationPane
                    navigationPane.push(view)
                 }
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
