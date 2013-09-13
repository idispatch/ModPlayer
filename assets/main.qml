import bb.cascades 1.0

TabbedPane {
    id: mainTabPane
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
        NavigationPane {
            id: myFavouriteNavigationPane
            SongList {
                id: songMyFavouriteView
                navigationPane: myFavouriteNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
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
        NavigationPane {
            id: recentlyPlayedNavigationPane 
            SongList {
                id: songRecentlyPlayedView
                navigationPane: recentlyPlayedNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
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
        NavigationPane {
            id: mostPlayedNavigationPane
            SongList {
                id: songMostPlayedView
                navigationPane: mostPlayedNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
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
        NavigationPane {
            id: searchNavigationPane
            SongList {
                id: songSearchView
                navigationPane: searchNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
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
        NavigationPane {
            id: topDownloadsNavigationPane
            SongList {
                id: songsTopDownloadedView
                navigationPane: topDownloadsNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
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
        NavigationPane {
            id: topFavouritedNavigationPane
            SongList {
                id: songTopFavouritedView
                navigationPane: topFavouritedNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
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
        NavigationPane {
            id: topScoredNavigationPane
            SongList {
                id: songTopScoredView
                navigationPane: topScoredNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
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
        }
        NavigationPane {
            id: artistsNavigationPane
            ArtistsList {
                id: songArtistsList
                navigationPane: artistsNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
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
        }
        NavigationPane {
            id: genresNavigationPane 
            GenresList {
                id: songGenresList
                navigationPane: genresNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
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
        function unload() {
        }
        NavigationPane {
            id: formatsNavigationPane
            FormatsList {
                id: songFormatsList
                navigationPane: formatsNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    activeTab: searchTab
    onCreationCompleted: {
        songSearchView.loadSearchSongs()
    } 
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
            onTriggered: {
                var view = helpView.createObject(mainTabPane)
                view.open()
            }
            attachedObjects: [
                ComponentDefinition {
                    id: helpView
                    source: "Help.qml"
                }
            ]
        }
        settingsAction: SettingsActionItem {
            id: settingsActionItem
            title: qsTr("Settings")
            onTriggered : {
                var view = settingsView.createObject(mainTabPane)
                view.open()
            }
            attachedObjects: [
                ComponentDefinition {
                    id: settingsView
                    source: "Settings.qml"
                }
            ] 
        }
        actions: [
            ActionItem {
                title: qsTr("Twit!")
                imageSource: "asset:///images/actions/icon_twitter.png"
                onTriggered: {
                    app.twit()
                }
            },
            ActionItem {
                title: qsTr("BBM")
                imageSource: "asset:///images/actions/icon_bbm.png"
                onTriggered: {
                    app.bbm()
                }
            },
            ActionItem {
                title: qsTr("Email Author")
                imageSource: "asset:///images/actions/icon_email.png"
                onTriggered: {
                    app.emailAuthor()
                }
            }
        ]
    }
}