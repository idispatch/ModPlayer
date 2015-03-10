import bb.cascades 1.3
import player 1.0

TabbedPane {
    id: mainTabPane
    objectName: "mainTabPane"
    activeTab: searchTab
    Tab {
        id: searchTab
        title: qsTr("Search") + Retranslate.onLanguageChanged
        objectName: "searchTab"
        imageSource: "asset:///images/actions/icon_search.png"
        description: qsTr("Search Songs") + Retranslate.onLanguageChanged
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "myFavouriteTab"
        imageSource: "asset:///images/actions/icon_myfavourite.png"
        description: qsTr("My Favourite Songs") + Retranslate.onLanguageChanged
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "localSongsTab"
        imageSource: "asset:///images/actions/icon_my_songs.png"
        description: qsTr("Songs on My Device") + Retranslate.onLanguageChanged
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "recentlyPlayedTab"
        imageSource: "asset:///images/actions/icon_recent.png"
        description: qsTr("Recently Played Songs") + Retranslate.onLanguageChanged
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "playlistsTab"
        description: qsTr("My Playlists") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_playlist.png"
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "albumsTab"
        description: qsTr("Albums") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_albums.png"
        onTriggered: {
            load()
        }
        function load() {
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
        id: filesTab
        title: qsTr("File System") + Retranslate.onLanguageChanged
        objectName: "filesTab"
        description: qsTr("Files") + Retranslate.onLanguageChanged
        imageSource: app.isExtendedVersion ? "asset:///images/actions/icon_files.png" : "asset:///images/actions/icon_lock.png"
        onTriggered: {
            load()
        }
        function load() {
            app.analytics.showPage(title)
            songFiles.load()
        }
        function unload() {
            songFiles.unload()
        }
        NavigationPane {
            id: filesNavigationPane 
            FilesList {
                id: songFiles
                navigationPane: filesNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: liveStreamTab
        title: "Internet Radio"
        objectName: "liveStreamTab"
        description: qsTr("Live Stream Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_radio.png"
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "difmTab"
        description: qsTr("Digitally Imported Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_difm.png"
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "skyfmTab"
        description: qsTr("SKY.FM Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_skyfm.png"
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "jazzRadioTab"
        description: qsTr("Jazz Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_jazzradio.png"
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "rockRadioTab"
        description: qsTr("Rock Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_rockradio.png"
        onTriggered: {
            load()
        }
        function load() {
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
        id: frescaFmTab
        title: qsTr("Fresca Radio") + Retranslate.onLanguageChanged
        objectName: "frescaFmTab"
        description: qsTr("Fresca Radio") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_fresca.png"
        onTriggered: {
            load()
        }
        function load() {
            app.analytics.showPage(title)
            frescaFM.load()
        }
        function unload() {
            frescaFM.unload()
        }
        NavigationPane {
            id: frescaNavigationPane 
            InternetRadioList {
                id: frescaFM
                channelList: "app/native/assets/fresca.json"
                navigationPane: frescaNavigationPane
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: mostPlayedTab
        title: qsTr("Most Played") + Retranslate.onLanguageChanged
        objectName: "mostPlayedTab"
        imageSource: "asset:///images/actions/icon_mostplayed.png"
        description: qsTr("Most Played Songs") + Retranslate.onLanguageChanged
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "topDownloadsTab"
        imageSource: "asset:///images/actions/icon_downloads.png"
        description: qsTr("Top Downloaded Songs") + Retranslate.onLanguageChanged
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "topFavouritedTab"
        imageSource: "asset:///images/actions/icon_favourite.png"
        description: qsTr("Top Favourited Songs") + Retranslate.onLanguageChanged
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "topScoredTab"
        imageSource: "asset:///images/actions/icon_score.png"
        description: qsTr("Top Scored Songs") + Retranslate.onLanguageChanged
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "artistsTab"
        description: qsTr("Songs By Artist") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_artists.png"
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "genresTab"
        description: qsTr("Songs By Genre") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_genres.png"
        onTriggered: {
            load()
        }
        function load() {
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
        objectName: "formatsTab"
        description: qsTr("Songs By Module Format") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/actions/icon_format.png"
        onTriggered: {
            load()
        }
        function load() {
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
    function initialLoad() {
        console.log("Initial view: " + app.initialView)
        if(app.initialView == "myFavouriteTab") {
            mainTabPane.activeTab = myFavouriteTab
        } else if(app.initialView == "localSongsTab") {
            mainTabPane.activeTab = localSongsTab
        } else if(app.initialView == "recentlyPlayedTab") {
            mainTabPane.activeTab = recentlyPlayedTab
        } else if(app.initialView == "playlistsTab") {
            mainTabPane.activeTab = playlistsTab
        } else if(app.initialView == "albumsTab") {
            mainTabPane.activeTab = albumsTab
        } else if(app.initialView == "filesTab") {
            mainTabPane.activeTab = filesTab
        } else if(app.initialView == "liveStreamTab") {
            mainTabPane.activeTab = liveStreamTab
        } else if(app.initialView == "difmTab") {
            mainTabPane.activeTab = difmTab
        } else if(app.initialView == "skyfmTab") {
            mainTabPane.activeTab = skyfmTab
        } else if(app.initialView == "jazzRadioTab") {
            mainTabPane.activeTab = jazzRadioTab
        } else if(app.initialView == "rockRadioTab") {
            mainTabPane.activeTab = rockRadioTab
        } else if(app.initialView == "frescaFmTab") {
            mainTabPane.activeTab = frescaFmTab
        } else if(app.initialView == "mostPlayedTab") {
            mainTabPane.activeTab = mostPlayedTab
        } else if(app.initialView == "topDownloadsTab") {
            mainTabPane.activeTab = topDownloadsTab
        } else if(app.initialView == "topFavouritedTab") {
            mainTabPane.activeTab = topFavouritedTab
        } else if(app.initialView == "topScoredTab") {
            mainTabPane.activeTab = topScoredTab
        } else if(app.initialView == "artistsTab") {
            mainTabPane.activeTab = artistsTab
        } else if(app.initialView == "genresTab") {
            mainTabPane.activeTab = genresTab
        } else if(app.initialView == "formatsTab") {
            mainTabPane.activeTab = formatsTab
        } else {
            mainTabPane.activeTab = searchTab
        }
        console.log("Initial view object: " + mainTabPane.activeTab)
        try {
            mainTabPane.activeTab.load()
        } catch(err) {
            mainTabPane.activeTab = searchTab
            mainTabPane.activeTab.load()
        }
    }
    onCreationCompleted: {
        app.catalog.upgradeCompleted.connect(initialLoad)
    }
    onActiveTabChanged: {
        var tabCount = mainTabPane.count()
        for (var i = 0; i < tabCount; ++i) {
            var tab = mainTabPane.at(i)
            if(tab != activeTab) {
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