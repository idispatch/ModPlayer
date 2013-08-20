import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: personalSongListPage
    property variant navigationPane
    titleBar: TitleBar {
        title: {
            if(songs.mode == "recent") return qsTr("Recently Played Songs")
            if(songs.mode == "myFavourite") return qsTr("My Favourite Songs")
            if(songs.mode == "mostPlayed") return qsTr("Most Played Songs")
            if(songs.mode == "topFavourited") return qsTr("Top Favourited Songs")
            if(songs.mode == "topScored") return qsTr("Top Scored Songs")
            if(songs.mode == "topDownloads") return qsTr("Top Downloaded Songs")
            return ""
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
            visible: false
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            topPadding: 20
            bottomPadding: topPadding
            leftPadding: 10
            rightPadding: leftPadding
            listItemComponents: [
                ListItemComponent {
                    ModPlayerListItem {
                        title: ListItem.data.title
                        description: ListItem.data.fileName
                        upperStatus: {
                            var mode = ListItem.view.mode
                            if(mode == "recent") return Global.formatTimeStamp(ListItem.data.lastPlayed)
                            if(mode == "myFavourite") return "played " + (ListItem.data.playCount == 1 ? "once" : (ListItem.data.playCount + " times"))
                            if(mode == "mostPlayed") return "played " + (ListItem.data.playCount == 1 ? "once" : (ListItem.data.playCount + " times"))
                            if(mode == "topFavourited") return "favourited " + ListItem.data.favourited + " times"
                            if(mode == "topScored") return "score " + ListItem.data.score + " of 10"
                            if(mode == "topDownloads") return ListItem.data.downloads + " downloads"
                            return ""
                        }
                        middleStatus: {
                            var mode = ListItem.view.mode
                            if(mode == "topDownloads") {
                                if(ListItem.data.score > 0) {
                                    return "score " + ListItem.data.score + " of 10"
                                }
                            }
                            if(mode == "topFavourited") {
                                if(ListItem.data.score > 0) {
                                    return "score " + ListItem.data.score + " of 10"
                                }
                            }
                            if(mode == "topScored") {
                                if(ListItem.data.downloads > 0) {
                                    return ListItem.data.downloads + " downloads"
                                }
                            }
                            return ""
                        }
                        lowerStatus: {
                            var mode = ListItem.view.mode
                            if(mode == "recent") return "played " + (ListItem.data.playCount == 1 ? "once" : (ListItem.data.playCount + " times"))
                            if(mode == "myFavourite") return Global.formatTimeStamp(ListItem.data.lastPlayed)
                            if(mode == "mostPlayed") return Global.formatTimeStamp(ListItem.data.lastPlayed)
                            if(mode == "topFavourited") {
                                if(ListItem.data.downloads > 0) {
                                    return ListItem.data.downloads + " downloads"
                                }
                            }
                            if(mode == "topScored") {
                                if(ListItem.data.favourited > 0) {
                                    return "favourited " + ListItem.data.favourited + " times"
                                }
                            }
                            if(mode == "topDownloads") {
                                if(ListItem.data.favourited > 0) {
                                    return "favourited " + ListItem.data.favourited + " times"
                                }
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
            console.log("P:destroyDataModel:=" + oldModel.size())
            oldModel.clear()
            songs.resetDataModel()
            oldModel = undefined
        }
    }
    function showList(listName, model) {
        songs.mode = listName
        destroyDataModel()
        songs.dataModel = model
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
    }
    function loadRecentlyPlayedSongs() {
        unload()
        showList("recent", app.player.catalog.findRecentlyPlayedSongs(personalSongListPage))
    }
    function loadMyFavouriteSongs() {
        unload()
        showList("myFavourite", app.player.catalog.findMyFavouriteSongs(personalSongListPage))
    }
    function loadMostPlayedSongs() {
        unload()
        showList("mostPlayed", app.player.catalog.findMostPlayedSongs(personalSongListPage))
    }
    function loadMostFavouritedSongs() {
        unload()
        showList("topFavourited", app.player.catalog.findMostFavouritedSongs(personalSongListPage))
    }
    function loadMostScoredSongs() {
        unload()
        showList("topScored", app.player.catalog.findMostScoredSongs(personalSongListPage))
    }
    function loadMostDownloadedSongs() {
        unload()
        showList("topDownloads", app.player.catalog.findMostDownloadedSongs(personalSongListPage))
    }
    actions: [
        PlayerActionItem {
            navigationPane: personalSongListPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        } 
    ]
}
