import bb.cascades 1.0
import player 1.0

Page {
    id: songListPage
    property variant navigationPane

    titleBar: TitleBar {
        title: {
            if(songs.mode == 'format')  return songs.modelName + " Songs";
            if(songs.mode == 'genre') return songs.modelName + " Songs";
            if(songs.mode == 'artist') return "Songs by " + songs.modelName;
            return "All Songs"
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
                        title: ListItem.data.title
                        description: ListItem.data.fileName
                        upperStatus: {
                            if(ListItem.data.score > 0) {
                                return "score " + ListItem.data.score + " of 10"
                            }
                            return ""
                        }
                        middleStatus: {
                            if(ListItem.data.favourited > 0) {
                                return "favourited " + ListItem.data.favourited + " times"
                            }
                            return ""
                        } 
                        lowerStatus: {
                            if(ListItem.data.downloads > 0) {
                                return ListItem.data.downloads + " downloads"
                            }
                            return ""
                        }  
                        imageSource: ListItem.data.iconPath
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
    function destroyDataModel() {
        if(songs.dataModel != null) {
            var oldModel = songs.dataModel
            console.log("S:destroyDataModel:=" + oldModel.size())
            songs.resetDataModel()
            oldModel.clear()
            oldModel = undefined
        }
    }
    function showList(listName, modelName, model) {
        songs.mode = listName
        songs.modelName = modelName
        destroyDataModel()
        songs.setDataModel(model)
        progress.running = false
        progress.visible = false
        songs.visible = true
    }
    function unload() {
        progress.running = true
        progress.visible = true
        songs.visible = false
        destroyDataModel()
        songs.mode = ""
        songs.modelName = ""
    }
    function loadSongsByFormat(formatId, formatName) {
        unload()
        showList("format", formatName, app.player.catalog.findSongsByFormatId(formatId, songListPage))
    }
    function loadSongsByGenre(genreId, genreName) {
        unload()
        showList("genre", genreName, app.player.catalog.findSongsByGenreId(genreId, songListPage))
    }
    function loadSongsByArtist(artistId, artistName) {
        unload()
        showList("artist", artistName, app.player.catalog.findSongsByArtistId(artistId, songListPage))
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
