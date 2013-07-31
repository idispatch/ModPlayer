import bb.cascades 1.1

TabbedPane {
    Tab {
        id: formatsTab
        title: "Formats"
        description: "Songs By Module Format"
        NavigationPane {
            FormatsList {
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
        NavigationPane {
            GenresList {
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
        settingsAction: SettingsActionItem {
            property variant settingsViewObject;
            id: settingsActionItem
            title: "Settings";
            onTriggered : {
                settingsViewObject = settingsView.createObject()
                settingsViewObject.open()
            }
        }
        
        actions: [  
            /*ActionItem {
                title: "Play"
                imageSource: "asset:///images/menuicons/icon_account.png"    
                onTriggered: {
                    Invoke.invokeExecute("","","invokeAccount");  // tab/page = "" : Current tab
                }
            },
            ActionItem {
                objectName: "appMenuInviteBBM"
                title: "Invite"
                imageSource: "asset:///images/menuicons/icon_bbm.png"
                onTriggered: {
                    BBMInvite.sendInvite();
                }
            }*/
        ] 
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: settingsView
            source: "Settings.qml"
        }
    ]
}