import bb.cascades 1.0

TabbedPane {
    id: mainTabPane
    objectName: "mainTabPane"
    Tab {
        id: searchTab
        title: qsTr("Search")
        objectName: title
        imageSource: "asset:///images/actions/icon_search.png"
        description: qsTr("Search Songs")
        onTriggered: {
            app.analytics.showPage(title)
            songSearchView.loadSearchSongs()
        }
        function unload() {
            songSearchView.unload()
        }
        content: NavigationPane {
            id: searchNavigationPane
            SongList {
                id: songSearchView
                navigationPane: searchNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: myFavouriteTab
        title: qsTr("My Favourite")
        objectName: title
        imageSource: "asset:///images/actions/icon_myfavourite.png"
        description: qsTr("My Favourite Songs")
        onTriggered: {
            app.analytics.showPage(title)
            songMyFavouriteView.loadMyFavouriteSongs()
        }
        function unload() {
            songMyFavouriteView.unload()
        }
        content: NavigationPane {
            id: myFavouriteNavigationPane
            SongList {
                id: songMyFavouriteView
                navigationPane: myFavouriteNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: localSongsTab
        title: qsTr("Local Songs")
        objectName: title
        imageSource: "asset:///images/actions/icon_my_songs.png"
        description: qsTr("Songs on My Device")
        onTriggered: {
            app.analytics.showPage(title)
            songMyLocalView.loadMyLocalSongs()
        }
        function unload() {
            songMyLocalView.unload()
        }
        content: NavigationPane {
            id: myLocalNavigationPane
            SongList {
                id: songMyLocalView
                navigationPane: myLocalNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: recentlyPlayedTab
        title: qsTr("Recent")
        objectName: title
        imageSource: "asset:///images/actions/icon_recent.png"
        description: qsTr("Recently Played Songs")
        onTriggered: {
            app.analytics.showPage(title)
            songRecentlyPlayedView.loadRecentlyPlayedSongs()
        }
        function unload() {
            songRecentlyPlayedView.unload()
        }
        content: NavigationPane {
            id: recentlyPlayedNavigationPane 
            SongList {
                id: songRecentlyPlayedView
                navigationPane: recentlyPlayedNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: playlistsTab
        title: qsTr("Playlists")
        objectName: title
        description: qsTr("My Playlists")
        imageSource: "asset:///images/actions/icon_playlist.png"
        onTriggered: {
            app.analytics.showPage(title)
            songPlaylists.load()
        }
        function unload() {
            songPlaylists.unload()
        }
        content: NavigationPane {
            id: playlistsNavigationPane 
            Playlists {
                id: songPlaylists
                navigationPane: playlistsNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: albumsTab
        title: qsTr("Albums")
        objectName: title
        description: qsTr("Albums")
        imageSource: "asset:///images/actions/icon_albums.png"
        onTriggered: {
            app.analytics.showPage(title)
            songAlbums.load()
        }
        function unload() {
            songAlbums.unload()
        }
        content: NavigationPane {
            id: albumsNavigationPane 
            Albums {
                id: songAlbums
                navigationPane: albumsNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: difmTab
        title: qsTr("Digitally Imported")
        objectName: title
        description: qsTr("Digitally Imported Radio")
        imageSource: "asset:///images/actions/icon_difm.png"
        onTriggered: {
            app.analytics.showPage(title)
            difm.load()
        }
        function unload() {
            songAlbums.unload()
        }
        content: NavigationPane {
            id: difmNavigationPane 
            Albums {
                id: difm
                navigationPane: difmNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: mostPlayedTab
        title: qsTr("Most Played")
        objectName: title
        imageSource: "asset:///images/actions/icon_mostplayed.png"
        description: qsTr("Most Played Songs")
        onTriggered: {
            app.analytics.showPage(title)
            songMostPlayedView.loadMostPlayedSongs()
        }
        function unload() {
            songMostPlayedView.unload()
        }
        content: NavigationPane {
            id: mostPlayedNavigationPane
            SongList {
                id: songMostPlayedView
                navigationPane: mostPlayedNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: topDownloadsTab
        title: qsTr("Downloads")
        objectName: title
        imageSource: "asset:///images/actions/icon_downloads.png"
        description: qsTr("Top Downloaded Songs")
        onTriggered: {
            app.analytics.showPage(title)
            songsTopDownloadedView.loadMostDownloadedSongs()
        }
        function unload() {
            songsTopDownloadedView.unload()
        }
        content: NavigationPane {
            id: topDownloadsNavigationPane
            SongList {
                id: songsTopDownloadedView
                navigationPane: topDownloadsNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: topFavouritedTab
        title: qsTr("Favourited")
        objectName: title
        imageSource: "asset:///images/actions/icon_favourite.png"
        description: qsTr("Top Favourited Songs")
        onTriggered: {
            app.analytics.showPage(title)
            songTopFavouritedView.loadMostFavouritedSongs()
        }
        function unload() {
            songTopFavouritedView.unload()
        }
        content: NavigationPane {
            id: topFavouritedNavigationPane
            SongList {
                id: songTopFavouritedView
                navigationPane: topFavouritedNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: topScoredTab
        title: qsTr("Scored")
        objectName: title
        imageSource: "asset:///images/actions/icon_score.png"
        description: qsTr("Top Scored Songs")
        onTriggered: {
            app.analytics.showPage(title)
            songTopScoredView.loadMostScoredSongs()
        }
        function unload() {
            songTopScoredView.unload()
        }
        content: NavigationPane {
            id: topScoredNavigationPane
            SongList {
                id: songTopScoredView
                navigationPane: topScoredNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: artistsTab
        title: qsTr("Artists")
        objectName: title
        description: qsTr("Songs By Artist")
        imageSource: "asset:///images/actions/icon_artists.png"
        onTriggered: {
            app.analytics.showPage(title)
            songArtistsList.load()
        }
        function unload() {
            songArtistsList.unload()
        }
        content: NavigationPane {
            id: artistsNavigationPane
            ArtistsList {
                id: songArtistsList
                navigationPane: artistsNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: genresTab
        title: qsTr("Genres")
        objectName: title
        description: qsTr("Songs By Genre")
        imageSource: "asset:///images/actions/icon_genres.png"
        onTriggered: {
            app.analytics.showPage(title)
            songGenresList.load()
        }
        function unload() {
            songGenresList.unload()
        }
        content: NavigationPane {
            id: genresNavigationPane 
            GenresList {
                id: songGenresList
                navigationPane: genresNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    Tab {
        id: formatsTab
        title: qsTr("Formats")
        objectName: title
        description: qsTr("Songs By Module Format")
        imageSource: "asset:///images/actions/icon_format.png"
        onTriggered: {
            app.analytics.showPage(title)
            songFormatsList.load()
        }
        function unload() {}
        content: NavigationPane {
            id: formatsNavigationPane
            FormatsList {
                id: songFormatsList
                navigationPane: formatsNavigationPane
            }
            onPopTransitionEnded: page.destroy()
        }
    }
    activeTab: searchTab
    onCreationCompleted: songSearchView.loadSearchSongs()
    onActiveTabChanged: {
        var c = mainTabPane.count()
        for (var i = 0; i < c; ++i) {
            var tab = mainTabPane.at(i)
            if(tab!=activeTab) {
                tab.unload()
            }
        }
    }
    Menu.definition: MenuDefinition {
        helpAction: HelpActionItem {
            id: helpActionItem
            title: qsTr("Help")
            enabled: true
            onTriggered: helpView.createObject(mainTabPane).open()
            attachedObjects: [
                ComponentDefinition {
                    id: helpView
                    source: "Help.qml"
                }
            ]
        }
        actions: [
            ActionItem {
                title: qsTr("Twit!")
                imageSource: "asset:///images/actions/icon_twitter.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: true
                onTriggered: app.twit()
            },
            ActionItem {
                title: qsTr("BBM")
                imageSource: "asset:///images/actions/icon_bbm.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: true
                onTriggered: app.bbm()
            },
            ActionItem {
                title: qsTr("Email Author")
                imageSource: "asset:///images/actions/icon_email.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: true
                onTriggered: app.emailAuthor()
            }
        ]
        settingsAction: SettingsActionItem {
            id: settingsActionItem
            title: qsTr("Settings")
            enabled: true
            onTriggered : settingsView.createObject(mainTabPane).open()
            attachedObjects: [
                ComponentDefinition {
                    id: settingsView
                    source: "Settings.qml"
                }
            ] 
        }
    }
}