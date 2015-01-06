import bb.cascades 1.0
import player 1.0

TabbedPane {
    id: mainTabPane
    objectName: "mainTabPane"
    Tab {
        id: searchTab
        title: qsTr("Search") + Retranslate.onLanguageChanged
        objectName: title
        imageSource: "asset:///images/actions/icon_search.png"
        description: qsTr("Search Songs") + Retranslate.onLanguageChanged
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
        id: myFavouriteTab
        title: qsTr("My Favourite") + Retranslate.onLanguageChanged
        objectName: title
        imageSource: "asset:///images/actions/icon_myfavourite.png"
        description: qsTr("My Favourite Songs") + Retranslate.onLanguageChanged
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
        id: localSongsTab
        title: qsTr("Local Songs") + Retranslate.onLanguageChanged
        objectName: title
        imageSource: "asset:///images/actions/icon_my_songs.png"
        description: qsTr("Songs on My Device") + Retranslate.onLanguageChanged
        onTriggered: {
            app.analytics.showPage(title)
            songMyLocalView.loadMyLocalSongs()
        }
        function unload() {
            songMyLocalView.unload()
        }
        NavigationPane {
            id: myLocalNavigationPane
            SongList {
                id: songMyLocalView
                navigationPane: myLocalNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: recentlyPlayedTab
        title: qsTr("Recent") + Retranslate.onLanguageChanged
        objectName: title
        imageSource: "asset:///images/actions/icon_recent.png"
        description: qsTr("Recently Played Songs") + Retranslate.onLanguageChanged
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
        id: playlistsTab
        title: qsTr("Playlists") + Retranslate.onLanguageChanged
        objectName: title
        description: qsTr("My Playlists") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_playlist.png"
        onTriggered: {
            app.analytics.showPage(title)
            songPlaylists.load()
        }
        function unload() {
            songPlaylists.unload()
        }
        NavigationPane {
            id: playlistsNavigationPane 
            Playlists {
                id: songPlaylists
                navigationPane: playlistsNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: albumsTab
        title: qsTr("Albums") + Retranslate.onLanguageChanged
        objectName: title
        description: qsTr("Albums") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_albums.png"
        onTriggered: {
            app.analytics.showPage(title)
            songAlbums.load()
        }
        function unload() {
            songAlbums.unload()
        }
        NavigationPane {
            id: albumsNavigationPane 
            Albums {
                id: songAlbums
                navigationPane: albumsNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: liveStreamTab
        title: "Internet Radio"
        objectName: title
        description: qsTr("Live Stream Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_radio.png"
        onTriggered: {
            app.analytics.showPage(title)
            liveStream.load()
        }
        function unload() {
            liveStream.unload()
        }
        NavigationPane {
            id: liveStreamNavigationPane 
            LiveStreamRadio {
                id: liveStream
                navigationPane: liveStreamNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: difmTab
        title: qsTr("Digitally Imported") + Retranslate.onLanguageChanged
        objectName: title
        description: qsTr("Digitally Imported Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_difm.png"
        onTriggered: {
            app.analytics.showPage(title)
            difm.load()
        }
        function unload() {
            difm.unload()
        }
        NavigationPane {
            id: difmNavigationPane 
            InternetRadioList {
                id: difm
                channelList: "app/native/assets/difm.json"
                navigationPane: difmNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: skyfmTab
        title: qsTr("SKY.FM") + Retranslate.onLanguageChanged
        objectName: title
        description: qsTr("SKY.FM Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_skyfm.png"
        onTriggered: {
            app.analytics.showPage(title)
            skyfm.load()
        }
        function unload() {
            skyfm.unload()
        }
        NavigationPane {
            id: skyfmNavigationPane 
            InternetRadioList {
                id: skyfm
                channelList: "app/native/assets/skyfm.json"
                navigationPane: skyfmNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: jazzRadioTab
        title: qsTr("Jazz Radio") + Retranslate.onLanguageChanged
        objectName: title
        description: qsTr("Jazz Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_jazzradio.png"
        onTriggered: {
            app.analytics.showPage(title)
            jazzRadio.load()
        }
        function unload() {
            jazzRadio.unload()
        }
        NavigationPane {
            id: jazzRadioNavigationPane 
            InternetRadioList {
                id: jazzRadio
                channelList: "app/native/assets/jazzradio.json"
                navigationPane: jazzRadioNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: rockRadioTab
        title: qsTr("Rock Radio") + Retranslate.onLanguageChanged
        objectName: title
        description: qsTr("Rock Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_rockradio.png"
        onTriggered: {
            app.analytics.showPage(title)
            rockRadio.load()
        }
        function unload() {
            rockRadio.unload()
        }
        NavigationPane {
            id: rockRadioNavigationPane 
            InternetRadioList {
                id: rockRadio
                channelList: "app/native/assets/rockradio.json"
                navigationPane: rockRadioNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: mostPlayedTab
        title: qsTr("Most Played") + Retranslate.onLanguageChanged
        objectName: title
        imageSource: "asset:///images/actions/icon_mostplayed.png"
        description: qsTr("Most Played Songs") + Retranslate.onLanguageChanged
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
        id: topDownloadsTab
        title: qsTr("Downloads") + Retranslate.onLanguageChanged
        objectName: title
        imageSource: "asset:///images/actions/icon_downloads.png"
        description: qsTr("Top Downloaded Songs") + Retranslate.onLanguageChanged
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
        title: qsTr("Favourited") + Retranslate.onLanguageChanged
        objectName: title
        imageSource: "asset:///images/actions/icon_favourite.png"
        description: qsTr("Top Favourited Songs") + Retranslate.onLanguageChanged
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
        title: qsTr("Scored") + Retranslate.onLanguageChanged
        objectName: title
        imageSource: "asset:///images/actions/icon_score.png"
        description: qsTr("Top Scored Songs") + Retranslate.onLanguageChanged
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
        title: qsTr("Artists") + Retranslate.onLanguageChanged
        objectName: title
        description: qsTr("Songs By Artist") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_artists.png"
        onTriggered: {
            app.analytics.showPage(title)
            songArtistsList.load()
        }
        function unload() {
            songArtistsList.unload()
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
        title: qsTr("Genres") + Retranslate.onLanguageChanged
        objectName: title
        description: qsTr("Songs By Genre") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_genres.png"
        onTriggered: {
            app.analytics.showPage(title)
            songGenresList.load()
        }
        function unload() {
            songGenresList.unload()
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
        title: qsTr("Formats") + Retranslate.onLanguageChanged
        objectName: title
        description: qsTr("Songs By Module Format") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_format.png"
        onTriggered: {
            app.analytics.showPage(title)
            songFormatsList.load()
        }
        function unload() {}
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
            title: qsTr("Help") + Retranslate.onLanguageChanged
            enabled: true
            onTriggered: {
                helpView.createObject(mainTabPane).open()
            }
            attachedObjects: [
                ComponentDefinition {
                    id: helpView
                    source: "Help.qml"
                }
            ]
        }
        actions: [
            ActionItem {
                title: qsTr("Twit!") + Retranslate.onLanguageChanged
                imageSource: "asset:///images/actions/icon_twitter.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: true
                onTriggered: {
                    app.twit()
                }
            },
            ActionItem {
                title: qsTr("BBM") + Retranslate.onLanguageChanged
                imageSource: "asset:///images/actions/icon_bbm.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: true
                onTriggered: {
                    app.bbm()
                }
            },
            ActionItem {
                title: qsTr("Email Author") + Retranslate.onLanguageChanged
                imageSource: "asset:///images/actions/icon_email.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: true
                onTriggered: {
                    app.emailAuthor()
                }
            }
        ]
        settingsAction: SettingsActionItem {
            id: settingsActionItem
            title: qsTr("Settings") + Retranslate.onLanguageChanged
            onTriggered : {
                settingsView.createObject(mainTabPane).open()
            }
            attachedObjects: [
                ComponentDefinition {
                    id: settingsView
                    source: "Settings.qml"
                }
            ] 
        }
    }
}