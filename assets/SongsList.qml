import bb.cascades 1.0
import player 1.0

Page {
    id: songListPage
    property variant navigationPane

    titleBar: TitleBar {
        title: {
            var c = songs.dataModel ? songs.dataModel.size() : 0
            if(songs.mode == 'format')  return qsTr("%1 Songs (%2)").arg(songs.modelName).arg(c)
            if(songs.mode == 'genre') return qsTr("%1 Songs (%2)").arg(songs.modelName).arg(c)
            if(songs.mode == 'artist') return qsTr("Songs by %1 (%2)").arg(songs.modelName).arg(c)
            return qsTr("All Songs (%1)").arg(c)
        }
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    ViewContainer {
        ProgressComponent {
            id: progress
        }
        ListView {
            id: songs
            property string mode
            property string modelName
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
                        title: ListItem.data ? ListItem.data.title : ""
                        description: ListItem.data ? ListItem.data.fileName : ""
                        upperStatus: {
                            if(ListItem.data && ListItem.data.score > 0) {
                                return qsTr("score %1 of 10").arg(ListItem.data.score)
                            }
                            return ""
                        }
                        middleStatus: {
                            if(ListItem.data && ListItem.data.favourited > 0) {
                                return qsTr("favourited %1 times").arg(ListItem.data.favourited)
                            }
                            return ""
                        } 
                        lowerStatus: {
                            if(ListItem.data && ListItem.data.downloads > 0) {
                                return qsTr("%1 downloads").arg(ListItem.data.downloads)
                            }
                            return ""
                        }  
                        imageSource: ListItem.data ? ListItem.data.iconPath : ""
                    }
                }
            ]
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songView.createObject()
                view.navigationPane = navigationPane
                view.load(chosenItem.id)
                navigationPane.push(view)
            }
            attachedObjects: [
                ComponentDefinition {
                    id: songView
                    source: "SongView.qml"
                }
            ]
        }
    }
    function showList(listName, modelName, model) {
        songs.mode = listName
        songs.modelName = modelName
        if(songs.dataModel) {
            songs.dataModel.clear()
        }
        songs.resetDataModel()
        songs.setDataModel(model)
        progress.running = false
        progress.visible = false
        songs.visible = true
    }
    function unload() {
        progress.running = true
        progress.visible = true
        songs.visible = false
        if(songs.dataModel) {
            songs.dataModel.clear()
        }
        songs.resetDataModel()
        songs.mode = ""
        songs.modelName = ""
    }
    function loadSongsByFormat(formatId, formatName) {
        unload()
        showList("format", formatName, app.player.catalog.findSongsByFormatId(formatId))
    }
    function loadSongsByGenre(genreId, genreName) {
        unload()
        showList("genre", genreName, app.player.catalog.findSongsByGenreId(genreId))
    }
    function loadSongsByArtist(artistId, artistName) {
        unload()
        showList("artist", artistName, app.player.catalog.findSongsByArtistId(artistId))
    }
    actions: [
        PlayerActionItem {
            navigationPane: songListPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        } 
    ]
}
