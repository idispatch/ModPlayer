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
    function showPlayerView() {
        if (mainTabPane.activePane == navigationPane && navigationPane.top == filesPage) {
            var view = songPlayer.createObject()
            if(view) {
                view.navigationPane = navigationPane
                navigationPane.push(view)
            }
        }
    }
    Container {
        layout: DockLayout {}
        WallpaperView {}
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            topPadding: 20
            bottomPadding: 20
            leftPadding: 20
            rightPadding: 20
            GroupContainer {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
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
                    verticalAlignment: VerticalAlignment.Center
                    topPadding: 20
                    bottomPadding: topPadding
                    leftPadding: 10
                    rightPadding: leftPadding
                    function playSong(songPath) {
                        if(app.isExtendedVersion) {
                            showPlayerView()
                            app.player.playlist.assign(songPath)
                            app.player.playPlaylist()
                        } else {
                            app.pleaseBuy()
                        }
                    }
                    listItemComponents: [
                        ListItemComponent {
                            FileEntryView {
                                id: fileEntry
                                itemIndex: ListItem.indexInSection
                                fileName: ListItem.data.fileName
                                fileSize: ListItem.data.fileSize
                                fileType: ListItem.data.fileType
                                fileIcon: ListItem.data.fileIcon
                                isDirectory: ListItem.data.isDirectory
                                contextActions: [
                                    ActionSet {
                                        title: fileEntry.ListItem.data.fileName
                                        subtitle: fileEntry.ListItem.data.fileName
                                        actions: [
                                            ActionItem {
                                                title: qsTr("Play") + Retranslate.onLanguageChanged
                                                enabled: fileEntry.ListItem.data.isFile
                                                imageSource: "asset:///images/actions/icon_play.png"
                                                onTriggered: {
                                                    fileEntry.ListItem.view.playSong(fileEntry.ListItem.data.filePath)
                                                }
                                            }
                                        ]
                                    }
                                ]
                            }
                        }
                    ]
                    onTriggered: {
                        var chosenItem = dataModel.data(indexPath)
                        if(chosenItem) {
                            if(chosenItem.isDirectory) {
                                var view = songList.createObject()
                                if(view) {
                                    activityIndicator.view = view
                                    activityIndicator.filePath = chosenItem.filePath
                                    activityIndicator.visible = true
                                    activityIndicator.start()
                                }
                            } else if(chosenItem.isFile) {
                                playSong(chosenItem.filePath)
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
                    visible: !filesList.dataModel || filesList.dataModel.size() == 0
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
        songFilesModel.clear()
        if(filesList.dataModel) {
            filesList.dataModel.clear()
        }
    }
    function load() {
        loadPath("/")
    }
    function loadPath(path) {
        songFilesModel.clear()
        filesList.dataModel = app.fileSystem.listFiles(path)
        var collection = []
        var numItems = filesList.dataModel.size()
        for(var i = 0; i < numItems; i++) {
            var songItem = filesList.dataModel.value(i)
            if(songItem.isFile) {
                collection.push(songItem.filePath)
            }
        }
        songFilesModel.append(collection)
        playAllActionItem.songList = songFilesModel
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
        playAllActionItem.playbackStarted.connect(showPlayerView)
    }
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        },
        ArrayDataModel {
            id: songFilesModel
        }
    ]
    actions: [
        PlayerActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            navigationPane: filesPage.navigationPane
        },
        PlayAllActionItem {
            id: playAllActionItem
            checkExtendedVersion: !app.isExtendedVersion
            ActionBar.placement: ActionBarPlacement.OnBar
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
