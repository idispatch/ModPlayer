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
    function showList(listName, model) {
        songs.mode = listName
        if(songs.dataModel != null) {
            var dataModel = songs.dataModel
            songs.dataModel = null
            dataModel.destroy()
        }
        songs.dataModel = model
        progress.running = false
        progress.visible = false
        songs.visible = true
    }
    function unload() {
        if(songs.dataModel != null) {
            var dataModel = songs.dataModel
            songs.dataModel = null
            //dataModel.destroy()
        }
        songs.visible = false
        songs.mode = ""
        progress.running = true
        progress.visible = true
    }
    function loadRecentlyPlayedSongs() {
        unload()
        showList("recent", app.player.catalog.findRecentlyPlayedSongs())
    }
    function loadMyFavouriteSongs() {
        unload()
        showList("myFavourite", app.player.catalog.findMyFavouriteSongs())
    }
    function loadMostPlayedSongs() {
        unload()
        showList("mostPlayed", app.player.catalog.findMostPlayedSongs())
    }
    function loadMostFavouritedSongs() {
        unload()
        showList("topFavourited", app.player.catalog.findMostFavouritedSongs())
    }
    function loadMostScoredSongs() {
        unload()
        showList("topScored", app.player.catalog.findMostScoredSongs())
    }
    function loadMostDownloadedSongs() {
        unload()
        showList("topDownloads", app.player.catalog.findMostDownloadedSongs())
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
