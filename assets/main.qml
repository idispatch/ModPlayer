import bb.cascades 1.0

TabbedPane {
    activeTab: formatsTab 
    Tab {
        id: myFavouriteTab
        title: "My Favourite"
        objectName: title
        imageSource: "asset:///images/icon_myfavourite.png"
        description: "My Favourite Songs"
        onTriggered: {
            songMyFavouriteView.load()
        }
        NavigationPane {
            SongsMyFavourite {
                id: songMyFavouriteView
                navigationPane: parent
            }
            
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: recentlyPlayedTab
        title: "Recent"
        objectName: title
        imageSource: "asset:///images/icon_recent.png"
        description: "Recently Played Songs"
        onTriggered: {
            songRecentlyPlayedView.load()
        }
        NavigationPane {
            SongsRecentlyPlayed {
                id: songRecentlyPlayedView
                navigationPane: parent
            }
            
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    
    Tab {
        id: mostPlayedTab
        title: "Most Played"
        objectName: title
        imageSource: "asset:///images/icon_mostplayed.png"
        description: "Most Played Songs"
        onTriggered: {
            songMostPlayedView.load()
        }
        NavigationPane {
            SongsMostPlayed {
                id: songMostPlayedView
                navigationPane: parent
            }
            
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: formatsTab
        title: "Formats"
        objectName: title
        description: "Songs By Module Format"
        imageSource: "asset:///images/icon_format.png"
        NavigationPane {
            FormatsList {
                id: songFormatsList
                navigationPane: parent
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: genresTab
        title: "Genres"
        objectName: title
        description: "Songs By Genre"
        imageSource: "asset:///images/icon_genre.png"
        NavigationPane {
            GenresList {
                id: songGenresList
                navigationPane: parent
            }
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: topDownloadsTab
        title: "Downloads"
        objectName: title
        imageSource: "asset:///images/icon_download.png"
        description: "Top Downloaded Songs"
        onTriggered: {
            songsTopDownloadedView.load()
        }
        NavigationPane {
            SongsTopDownloaded {
                id: songsTopDownloadedView
                navigationPane: parent
            }
            
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: topFavouritedTab
        title: "Favourited"
        objectName: title
        imageSource: "asset:///images/icon_heart.png"
        description: "Top Favourited Songs"
        onTriggered: {
            songTopFavouritedView.load()
        }
        NavigationPane {
            SongsTopFavourited {
                id: songTopFavouritedView
                navigationPane: parent
            }
            
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id: topScoredTab
        title: "Scored"
        objectName: title
        imageSource: "asset:///images/icon_score.png"
        description: "Top Scored Songs"
        onTriggered: {
            songTopScoredView.load()
        }
        NavigationPane {
            SongsTopScored {
                id: songTopScoredView
                navigationPane: parent
            }
            
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    
    Menu.definition: MenuDefinition {
        helpAction: HelpActionItem {
            id: helpActionItem
            title: "Help"
            onTriggered: {
            }
        }
        
        settingsAction: SettingsActionItem {
            id: settingsActionItem
            title: "Settings";
            onTriggered : {
                var view = settingsView.createObject()
                view.open()
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