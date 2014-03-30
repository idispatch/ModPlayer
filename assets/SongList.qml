import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: songListPage
    objectName: "songListPage"
    property variant navigationPane
    property int maxResults
    property int requestId
    property int listId
    titleBar: PlayerTitleBar {
        id: titleBar
        title: {
            var c = songs.dataModel ? songs.dataModel.size() : 0
            if(c > maxResults) {
                c = "%1+".arg(maxResults)
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
            if (songs.mode == 'album') return qsTr("%1 Album Songs (%2)").arg(songs.modelName).arg(c)
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
                    hintText: qsTr("search songs")
                    onSearch: {
                        unload()
                        load()
                    }
                }
                expanded: true
                indicatorVisibility: TitleBarExpandableAreaIndicatorVisibility.Visible
                toggleArea: TitleBarExpandableAreaToggleArea.EntireTitleBar
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
                if(mode == 'playlist') {
                    loadSongsByPlaylist(listId, modelName)
                } else if(mode == 'album') {
                    loadSongsByAlbum(listId, modelName)
                }
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
                                if (mode == "topScored" || mode == "format" || mode == "genre" || mode == "artist" || mode == "playlist" || mode == "album") {
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
                                if (mode == "format" || mode == "genre" || mode == "artist" || mode == "playlist" || mode == "album") {
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
                                if (mode == "topFavourited" || mode == "format" || mode == "genre" || mode == "artist" || mode == "playlist" || mode == "album") {
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
    function unload() {
        progress.start()
        songs.visible = false
        listEmpty.visible = false
        if (songs.dataModel) {
            songs.dataModel.clear()
        }
        songs.resetDataModel()
    }
    function load() {
        unload();
        maxResults = searchArea.searchTerm.length > 0 ? 100 : 5000
        requestId = {
            search: app.player.catalog.searchSongsAsync,
            recent: app.player.catalog.findRecentlyPlayedSongsAsync,
            myFavourite: app.player.catalog.findMyFavouriteSongsAsync,
            myLocal: app.player.catalog.findMyLocalSongsAsync,
            mostPlayed: app.player.catalog.findMostPlayedSongsAsync,
            topFavourited: app.player.catalog.findMostFavouritedSongsAsync,
            topScored: app.player.catalog.findMostScoredSongsAsync,
            topDownloads: app.player.catalog.findMostDownloadedSongsAsync,
            format: function(searchTerm, maxResults) { app.player.catalog.findSongsByFormatIdAsync(searchTerm, listId, maxResults) },
            genre: function(searchTerm, maxResults) { app.player.catalog.findSongsByGenreIdAsync(searchTerm, listId, maxResults) },
            artist: function(searchTerm, maxResults) { app.player.catalog.findSongsByArtistIdAsync(searchTerm, listId, maxResults) },
            playlist: function(searchTerm, maxResults) { app.player.catalog.findSongsByPlaylistIdAsync(searchTerm, listId, maxResults) },
            album: function(searchTerm, maxResults) { app.player.catalog.findSongsByAlbumIdAsync(searchTerm, listId, maxResults) }
        }[songs.mode](searchArea.searchTerm, maxResults)
    }
    function loadRecentlyPlayedSongs() {
        listId = 0
        songs.mode = "recent"
        songs.modelName = ""
        load()
    }
    function loadMyFavouriteSongs() {
        listId = 0
        songs.mode = "myFavourite"
        songs.modelName = ""
        load()
    }
    function loadMyLocalSongs() {
        listId = 0
        songs.mode = "myLocal"
        songs.modelName = ""
        load()
    }
    function loadMostPlayedSongs() {
        listId = 0
        songs.mode = "mostPlayed"
        songs.modelName = ""
        load()
    }
    function loadMostFavouritedSongs() {
        listId = 0
        songs.mode = "topFavourited"
        songs.modelName = ""
        load()
    }
    function loadMostScoredSongs() {
        listId = 0
        songs.mode = "topScored"
        songs.modelName = ""
        load()
    }
    function loadMostDownloadedSongs() {
        listId = 0
        songs.mode = "topDownloads"
        songs.modelName = ""
        load()
    }
    function loadSearchSongs() {
        listId = 0
        songs.mode = "search"
        songs.modelName = ""
        load()
    }
    function loadSongsByFormat(formatId, formatName) {
        listId = formatId
        songs.mode = "format"
        songs.modelName = formatName
        load()
    }
    function loadSongsByGenre(genreId, genreName) {
        listId = genreId
        songs.mode = "genre"
        songs.modelName = genreName
        load()
    }
    function loadSongsByArtist(artistId, artistName) {
        listId = artistId
        songs.mode = "artist"
        songs.modelName = artistName
        load()
    }
    function loadSongsByPlaylist(playlistId, playlistName) {
        listId = playlistId
        songs.mode = "playlist"
        songs.modelName = playlistName
        load()
    }
    function loadSongsByAlbum(albumId, albumName) {
        listId = albumId
        songs.mode = "album"
        songs.modelName = albumName
        load()
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
            navigationPane: songListPage.navigationPane
            playlistId: songs.mode == 'playlist' ? listId : null
            albumId: songs.mode == 'album' ? listId : null  
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
        PlayModeActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
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
