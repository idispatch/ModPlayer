import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: songListPage
    objectName: "songListPage"
    property variant navigationPane
    property int maximumSearchSongResults: 100
    property int requestId
    property int listId
    titleBar: PlayerTitleBar {
        id: titleBar
        title: {
            var c = songs.dataModel ? songs.dataModel.size() : 0
            if(c > maximumSearchSongResults) {
                c = "%1+".arg(maximumSearchSongResults)
            }
            if (songs.mode == 'recent') return qsTr("Recently Played Songs (%1)").arg(c)
            if (songs.mode == 'myFavourite') return qsTr("My Favourite Songs (%1)").arg(c)
            if (songs.mode == 'myLocal') return qsTr("Songs on My Device (%1)").arg(c)
            if (songs.mode == 'mostPlayed') return qsTr("Most Played Songs (%1)").arg(c)
            if (songs.mode == 'topFavourited') return qsTr("Top Favourited Songs (%1)").arg(c)
            if (songs.mode == 'topScored') return qsTr("Top Scored Songs (%1)").arg(c)
            if (songs.mode == 'topDownloads') return qsTr("Top Downloaded Songs (%1)").arg(c)
            if (songs.mode == 'format')  return qsTr("%1 Songs (%2)").arg(songs.modelName).arg(c)
            if (songs.mode == 'genre') return qsTr("%1 Songs (%2)").arg(songs.modelName).arg(c)
            if (songs.mode == 'artist') return qsTr("Songs by %1 (%2)").arg(songs.modelName).arg(c)
            if (songs.mode == 'playlist') return qsTr("%1 Playlist Songs (%2)").arg(songs.modelName).arg(c)
            if (songs.mode == 'search') {
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
                        showList("search", "", 
                                 app.player.catalog.searchSongsAsync(searchArea.searchTerm,
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
                        return TitleBarExpandableAreaToggleArea.IndicatorOnly
                    }
                }
            }
        }
    }
    ViewContainer {
        ProgressComponent {
            id: progress
        }
        HorizontalContainer{
            id: listEmpty
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Fill
            Label {
                text: qsTr("No songs in this list")
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                textStyle {
                    fontWeight: FontWeight.Bold
                    fontSize: FontSize.Large
                    color: Color.White
                }
            }
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
            function updateList() {
                loadSongsByPlaylist(listId, modelName)
            }
            function playSong(song) {
                showPlayerView()
                app.player.playlist.clear()
                app.player.playlist.add(song.id)
                app.player.playPlaylist()
            }
            function addFavourite(song) {
                app.catalog.addFavourite(song)
            }
            function removeFavourite(song) {
                app.catalog.removeFavourite(song)
            }
            function deleteSongFromPlaylist(song) {
                app.catalog.deleteSongFromPlaylist(listId, song.id)
            }
            function getRootObject() {
                return app
            }
            listItemComponents: [
                ListItemComponent {
                    ModPlayerListItem {
                        id: songEntry
                        title: ListItem.data.title
                        description: Global.fileNameOnly(ListItem.data.fileName)
                        text: "%1   (%2)".arg(ListItem.data.songLengthText).arg(Global.getSizeKb(ListItem.data.fileSize)) 
                        upperStatus: {
                            var mode = ListItem.view.mode
                            if (ListItem.data) {
                                if (mode == "recent") {
                                    if(ListItem.data.lastPlayed > 0) {
                                        return Global.formatTimeStamp(ListItem.data.lastPlayed)
                                    }
                                }
                                if (mode == "myFavourite" || mode == "mostPlayed" || mode == "myLocal") {
                                    if(ListItem.data.playCount > 0) {
                                        return "played " + (ListItem.data.playCount == 1 ? "once" : (ListItem.data.playCount + " times"))
                                    }
                                }
                                if (mode == "topFavourited") {
                                    if (ListItem.data.favourited > 0) {
                                        return qsTr("favourited %1 times").arg(ListItem.data.favourited)
                                    }
                                }
                                if (mode == "topScored" || mode == "format" || mode == "genre" || mode == "artist" || mode == "playlist") {
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
                                if (mode == "format" || mode == "genre" || mode == "artist" || mode == "playlist") {
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
                                if (mode == "myFavourite" || mode == "mostPlayed" || mode == "myLocal") {
                                    if(ListItem.data.lastPlayed > 0) {
                                        return Global.formatTimeStamp(ListItem.data.lastPlayed)
                                    }
                                }
                                if (mode == "topFavourited" || mode == "format" || mode == "genre" || mode == "artist" || mode == "playlist") {
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
                        contextActions: [
                            ActionSet {
                                title: songEntry.ListItem.data.title
                                subtitle: Global.fileNameOnly(songEntry.ListItem.data.fileName)
                                actions: [
                                    ActionItem {
                                        title: qsTr("Play")
                                        imageSource: "asset:///images/actions/icon_play.png"
                                        onTriggered: {
                                            songEntry.ListItem.view.playSong(songEntry.ListItem.data)
                                        }
                                    },
                                    AppendPlaylistActionItem {
                                        currentSong: songEntry.ListItem.data
                                        mode: songEntry.ListItem.view.mode
                                        rootObject: songEntry.ListItem.view.getRootObject() 
                                    },
                                    ActionItem {
                                        title: qsTr("Add to Favourites")
                                        imageSource: "asset:///images/actions/icon_like.png"
                                        onTriggered: {
                                            songEntry.ListItem.view.addFavourite(songEntry.ListItem.data)
                                        }
                                    },
                                    ActionItem {
                                        title: qsTr("Remove from Favourites")
                                        imageSource: "asset:///images/actions/icon_unlike.png"
                                        onTriggered: {
                                            songEntry.ListItem.view.removeFavourite(songEntry.ListItem.data)
                                        }
                                    },
                                    DeleteActionItem {
                                        enabled: songEntry.ListItem.view.mode == 'playlist'
                                        onTriggered: {
                                            songEntry.ListItem.view.deleteSongFromPlaylist(songEntry.ListItem.data)
                                            songEntry.ListItem.view.updateList()
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
    function showList(listName, modelName, request) {
        requestId = request
        songs.mode = listName
        songs.modelName = modelName
        if(songs.dataModel) {
            songs.dataModel.clear()
        }
        songs.resetDataModel()
    }
    function unload() {
        progress.start()
        songs.visible = false
        listEmpty.visible = false
        if (songs.dataModel) {
            songs.dataModel.clear()
        }
        songs.resetDataModel()
        songs.mode = ""
        songs.modelName = ""
    }
    function loadRecentlyPlayedSongs() {
        unload()
        listId = 0
        maximumSearchSongResults = 5000
        showList("recent", "", app.player.catalog.findRecentlyPlayedSongsAsync(maximumSearchSongResults))
    }
    function loadMyFavouriteSongs() {
        unload()
        listId = 0
        maximumSearchSongResults = 5000
        showList("myFavourite", "", app.player.catalog.findMyFavouriteSongsAsync(maximumSearchSongResults))
    }
    function loadMyLocalSongs() {
        unload()
        listId = 0
        maximumSearchSongResults = 5000
        showList("myLocal", "", app.player.catalog.findMyLocalSongsAsync(maximumSearchSongResults))
    }
    function loadMostPlayedSongs() {
        unload()
        listId = 0
        maximumSearchSongResults = 5000
        showList("mostPlayed", "", app.player.catalog.findMostPlayedSongsAsync(maximumSearchSongResults))
    }
    function loadMostFavouritedSongs() {
        unload()
        listId = 0
        maximumSearchSongResults = 5000
        showList("topFavourited", "", app.player.catalog.findMostFavouritedSongsAsync(maximumSearchSongResults))
    }
    function loadMostScoredSongs() {
        unload()
        listId = 0
        maximumSearchSongResults = 5000
        showList("topScored", "", app.player.catalog.findMostScoredSongsAsync(maximumSearchSongResults))
    }
    function loadMostDownloadedSongs() {
        unload()
        listId = 0
        maximumSearchSongResults = 5000
        showList("topDownloads", "", app.player.catalog.findMostDownloadedSongsAsync(maximumSearchSongResults))
    }
    function loadSearchSongs() {
        unload()
        listId = 0
        maximumSearchSongResults = 100
        showList("search", "", app.player.catalog.searchSongsAsync(searchArea.searchTerm, maximumSearchSongResults))
    }
    function loadSongsByFormat(formatId, formatName) {
        unload()
        listId = formatId
        maximumSearchSongResults = 5000
        showList("format", formatName, app.player.catalog.findSongsByFormatIdAsync(formatId, maximumSearchSongResults))
    }
    function loadSongsByGenre(genreId, genreName) {
        unload()
        listId = genreId
        maximumSearchSongResults = 5000
        showList("genre", genreName, app.player.catalog.findSongsByGenreIdAsync(genreId, maximumSearchSongResults))
    }
    function loadSongsByArtist(artistId, artistName) {
        unload()
        listId = artistId
        maximumSearchSongResults = 5000
        showList("artist", artistName, app.player.catalog.findSongsByArtistIdAsync(artistId, maximumSearchSongResults))
    }
    function loadSongsByPlaylist(playlistId, playlistName) {
        unload()
        listId = playlistId
        maximumSearchSongResults = 5000
        showList("playlist", playlistName, app.player.catalog.findSongsByPlaylistIdAsync(playlistId, maximumSearchSongResults))
    }
    function showPlayerView() {
        if(mainTabPane.activePane == navigationPane && 
           navigationPane.top == songListPage) {
            var view = songPlayer.createObject()
            view.navigationPane = navigationPane
            navigationPane.push(view)
        }
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(function() {
            showPlayerView()
        })
        app.catalog.resultReady.connect(function(responseId, result) {
            if(responseId != requestId) 
                return
            requestId = 0
            songs.dataModel = result
            progress.stop()
            songs.visible = (songs.dataModel.size() > 0)
            listEmpty.visible = (songs.dataModel.size() == 0)
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
            navigationPane: songListPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            playlistId: songs.mode == 'playlist' ? listId : null  
        },
        NextActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        PreviousActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppendPlaylistActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            songList: songs.dataModel
            mode: songs.mode 
        },
        ImportSongsActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        OpenSongActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
}
