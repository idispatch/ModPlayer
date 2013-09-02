import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: personalSongListPage
    property variant navigationPane
    property int maximumSearchSongResults: 500
    titleBar: PlayerTitleBar {
        id: titleBar
        title: {
            var c = songs.dataModel ? songs.dataModel.size() : 0
            if (songs.mode == "recent") return qsTr("Recently Played Songs (%1)").arg(c)
            if (songs.mode == "myFavourite") return qsTr("My Favourite Songs (%1)").arg(c)
            if (songs.mode == "mostPlayed") return qsTr("Most Played Songs (%1)").arg(c)
            if (songs.mode == "topFavourited") return qsTr("Top Favourited Songs (%1)").arg(c)
            if (songs.mode == "topScored") return qsTr("Top Scored Songs (%1)").arg(c)
            if (songs.mode == "topDownloads") return qsTr("Top Downloaded Songs (%1)").arg(c)
            if (songs.mode == "search") {
                if(c > maximumSearchSongResults) {
                    c = "%1+".arg(maximumSearchSongResults)
                }
                if(searchArea.searchTerm.length > 0) {
                    return qsTr("Search Songs (%1) - '%2'").arg(c).arg(searchArea.searchTerm)
                }
                return qsTr("Search Songs (%1)").arg(c)
            }
            return ""
        }
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            HorizontalContainer {
                leftPadding: 10
                Label {
                    text: titleBar.title
                    textStyle {
                        color: Color.White 
                        fontSize: FontSize.Large
                    }
                    verticalAlignment: VerticalAlignment.Center
                }
            }
            expandableArea {
                content: SearchArea {
                    id: searchArea
                    onSearch: {
                        unload()
                        showList("search", 
                                 app.player.catalog.searchSongs(searchArea.searchTerm,
                                                                maximumSearchSongResults))
                    }
                }
                expanded: songs.mode == "search"
                indicatorVisibility: {
                    if(songs.mode == "search") {
                        return TitleBarExpandableAreaIndicatorVisibility.Visible
                    } else {
                        return TitleBarExpandableAreaIndicatorVisibility.Hidden
                    }
                }
                toggleArea: {
                    if(songs.mode == "search") {
                        return TitleBarExpandableAreaToggleArea.EntireTitleBar
                    } else {
                        return TitleBarExpandableAreaIndicatorVisibility.IndicatorOnly
                    }
                }
            }
        }
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
                            if (ListItem.data) {
                                if (mode == "recent") {
                                    return Global.formatTimeStamp(ListItem.data.lastPlayed)
                                }
                                if (mode == "myFavourite" || mode == "mostPlayed") {
                                    return "played " + (ListItem.data.playCount == 1 ? "once" : (ListItem.data.playCount + " times"))
                                }
                                if (mode == "topFavourited") {
                                    return qsTr("favourited %1 times").arg(ListItem.data.favourited)
                                }
                                if (mode == "topScored") {
                                    return qsTr("score %1 of 10").arg(ListItem.data.score)
                                }
                                if (mode == "topDownloads" || mode == "search") {
                                    return qsTr("%1 downloads").arg(ListItem.data.downloads)
                                }
                            }
                            return ""
                        }
                        middleStatus: {
                            var mode = ListItem.view.mode
                            if (ListItem.data) {
                                if (mode == "topDownloads" || mode == "topFavourited" || mode == "search") {
                                    if (ListItem.data.score > 0) {
                                        return qsTr("score %1 of 10").arg(ListItem.data.score)
                                    }
                                }
                                if (mode == "topScored") {
                                    if (ListItem.data.downloads > 0) {
                                        return qsTr("%1 downloads").arg(ListItem.data.downloads)
                                    }
                                }
                            }
                            return ""
                        }
                        lowerStatus: {
                            var mode = ListItem.view.mode
                            if (ListItem.data) {
                                if (mode == "recent") {
                                    return "played " + (ListItem.data.playCount == 1 ? "once" : (ListItem.data.playCount + " times"))
                                }
                                if (mode == "myFavourite" || mode == "mostPlayed") {
                                    return Global.formatTimeStamp(ListItem.data.lastPlayed)
                                }
                                if (mode == "topFavourited") {
                                    if (ListItem.data.downloads > 0) {
                                        return qsTr("%1 downloads").arg(ListItem.data.downloads)
                                    }
                                }
                                if (mode == "topScored" || mode == "topDownloads" || mode == "search") {
                                    if (ListItem.data.favourited > 0) {
                                        return qsTr("favourited %1 times").arg(ListItem.data.favourited)
                                    }
                                }
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
    function showList(listName, model) {
        songs.mode = listName
        if (songs.dataModel) {
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
        if (songs.dataModel) {
            songs.dataModel.clear()
        }
        songs.resetDataModel()
        songs.mode = ""
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
    function loadSearchSongs() {
        unload()
        showList("search", app.player.catalog.searchSongs("", maximumSearchSongResults))
    }
    actions: [
        PlayerActionItem {
            navigationPane: personalSongListPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem {
        }
    ]
}
