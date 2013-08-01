import bb.cascades 1.1

TabbedPane {
    Tab {
        id: formatsTab
        title: "Formats"
        description: "Songs By Module Format"
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
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: settingsView
            source: "Settings.qml"
        }
    ]
}