import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: personalSongListPage
    property variant navigationPane
    property int maximumSearchSongResults: 100
    titleBar: PlayerTitleBar {
        id: titleBar
        title: {
            var c = songs.dataModel ? songs.dataModel.size() : 0
            if(c > maximumSearchSongResults) {
                c = "%1+".arg(maximumSearchSongResults)
            }
            if (songs.mode == "recent") return qsTr("Recently Played Songs (%1)").arg(c)
            if (songs.mode == "myFavourite") return qsTr("My Favourite Songs (%1)").arg(c)
            if (songs.mode == "mostPlayed") return qsTr("Most Played Songs (%1)").arg(c)
            if (songs.mode == "topFavourited") return qsTr("Top Favourited Songs (%1)").arg(c)
            if (songs.mode == "topScored") return qsTr("Top Scored Songs (%1)").arg(c)
            if (songs.mode == "topDownloads") return qsTr("Top Downloaded Songs (%1)").arg(c)
            if (songs.mode == 'format')  return qsTr("%1 Songs (%2)").arg(songs.modelName).arg(c)
            if (songs.mode == 'genre') return qsTr("%1 Songs (%2)").arg(songs.modelName).arg(c)
            if (songs.mode == 'artist') return qsTr("Songs by %1 (%2)").arg(songs.modelName).arg(c)
            if (songs.mode == "search") {
                if(searchArea.searchTerm.length > 0) {
                    return qsTr("Search Songs (%1) - '%2'").arg(c).arg(searchArea.searchTerm)
                }
                return qsTr("Search Songs (%1)").arg(c)
            }
            return qsTr("All Songs (%1)").arg(c)
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
                        maximumSearchSongResults = 100
                        showList("search",
                                 "", 
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
            property string modelName
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
                                    if(ListItem.data.lastPlayed > 0) {
                                        return Global.formatTimeStamp(ListItem.data.lastPlayed)
                                    }
                                }
                                if (mode == "myFavourite" || mode == "mostPlayed") {
                                    if(ListItem.data.playCount > 0) {
                                        return "played " + (ListItem.data.playCount == 1 ? "once" : (ListItem.data.playCount + " times"))
                                    }
                                }
                                if (mode == "topFavourited") {
                                    if (ListItem.data.favourited > 0) {
                                        return qsTr("favourited %1 times").arg(ListItem.data.favourited)
                                    }
                                }
                                if (mode == "topScored" || mode == "format" || mode == "genre" || mode == "artist") {
                                    if (ListItem.data.score > 0) {
                                        return qsTr("score %1 of 10").arg(ListItem.data.score)
                                    }
                                }
                                if (mode == "topDownloads" || mode == "search") {
                                    if (ListItem.data.downloads > 0) {
                                        return qsTr("%1 downloads").arg(ListItem.data.downloads)
                                    }
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
                                if (mode == "format" || mode == "genre" || mode == "artist") {
                                    if(ListItem.data.favourited > 0) {
                                        return qsTr("favourited %1 times").arg(ListItem.data.favourited)
                                    }
                                }
                            }
                            return ""
                        }
                        lowerStatus: {
                            var mode = ListItem.view.mode
                            if (ListItem.data) {
                                if (mode == "recent") {
                                    if(ListItem.data.playCount > 0) {
                                        return "played " + (ListItem.data.playCount == 1 ? "once" : (ListItem.data.playCount + " times"))
                                    }
                                }
                                if (mode == "myFavourite" || mode == "mostPlayed") {
                                    if(ListItem.data.lastPlayed > 0) {
                                        return Global.formatTimeStamp(ListItem.data.lastPlayed)
                                    }
                                }
                                if (mode == "topFavourited" || mode == "format" || mode == "genre" || mode == "artist") {
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
    function showList(listName, modelName, model) {
        songs.mode = listName
        songs.modelName = modelName
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
        songs.modelName = ""
    }
    function loadRecentlyPlayedSongs() {
        unload()
        maximumSearchSongResults = 1000
        showList("recent", "", app.player.catalog.findRecentlyPlayedSongs(maximumSearchSongResults))
    }
    function loadMyFavouriteSongs() {
        unload()
        maximumSearchSongResults = 1000
        showList("myFavourite", "", app.player.catalog.findMyFavouriteSongs(maximumSearchSongResults))
    }
    function loadMostPlayedSongs() {
        unload()
        maximumSearchSongResults = 1000
        showList("mostPlayed", "", app.player.catalog.findMostPlayedSongs(maximumSearchSongResults))
    }
    function loadMostFavouritedSongs() {
        unload()
        maximumSearchSongResults = 5000
        showList("topFavourited", "", app.player.catalog.findMostFavouritedSongs(maximumSearchSongResults))
    }
    function loadMostScoredSongs() {
        unload()
        maximumSearchSongResults = 5000
        showList("topScored", "", app.player.catalog.findMostScoredSongs(maximumSearchSongResults))
    }
    function loadMostDownloadedSongs() {
        unload()
        maximumSearchSongResults = 5000
        showList("topDownloads", "", app.player.catalog.findMostDownloadedSongs(maximumSearchSongResults))
    }
    function loadSearchSongs() {
        unload()
        maximumSearchSongResults = 100
        showList("search", "", app.player.catalog.searchSongs(searchArea.searchTerm, maximumSearchSongResults))
    }
    function loadSongsByFormat(formatId, formatName) {
        unload()
        maximumSearchSongResults = 5000
        showList("format", formatName, app.player.catalog.findSongsByFormatId(formatId, maximumSearchSongResults))
    }
    function loadSongsByGenre(genreId, genreName) {
        unload()
        maximumSearchSongResults = 5000
        showList("genre", genreName, app.player.catalog.findSongsByGenreId(genreId, maximumSearchSongResults))
    }
    function loadSongsByArtist(artistId, artistName) {
        unload()
        maximumSearchSongResults = 5000
        showList("artist", artistName, app.player.catalog.findSongsByArtistId(artistId, maximumSearchSongResults))
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
