import bb.cascades 1.0

TabbedPane {
    id: mainTabPane
    activeTab: formatsTab 
    Tab {
        id: myFavouriteTab
        title: qsTr("My Favourite")
        objectName: title
        imageSource: "asset:///images/actions/icon_myfavourite.png"
        description: qsTr("My Favourite Songs")
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
        title: qsTr("Recent")
        objectName: title
        imageSource: "asset:///images/actions/icon_recent.png"
        description: qsTr("Recently Played Songs")
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
        title: qsTr("Most Played")
        objectName: title
        imageSource: "asset:///images/actions/icon_mostplayed.png"
        description: qsTr("Most Played Songs")
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
        title: qsTr("Formats")
        objectName: title
        description: qsTr("Songs By Module Format")
        imageSource: "asset:///images/actions/icon_format.png"
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
        title: qsTr("Genres")
        objectName: title
        description: qsTr("Songs By Genre")
        imageSource: "asset:///images/actions/icon_genres.png"
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
        id: artistsTab
        title: qsTr("Artists")
        objectName: title
        description: qsTr("Songs By Artist")
        imageSource: "asset:///images/actions/icon_artists.png"
        NavigationPane {
            ArtistsList {
                id: songArtistsList
                navigationPane: parent
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
        title: qsTr("Favourited")
        objectName: title
        imageSource: "asset:///images/actions/icon_favourite.png"
        description: qsTr("Top Favourited Songs")
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
        title: qsTr("Scored")
        objectName: title
        imageSource: "asset:///images/actions/icon_score.png"
        description: qsTr("Top Scored Songs")
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
                title: qsTr("Email Author")
                imageSource: "asset:///images/actions/icon_email.png"
                onTriggered: {
                    app.emailAuthor()
                }
            }
        ]
    }
}