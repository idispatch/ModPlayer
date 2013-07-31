import bb.cascades 1.1

TabbedPane {
    id: rootTabbedPane

    Tab {
        id: formatsTab
        title: "Formats"
        description : "Song By Module Format"
        NavigationPane {
            id: songFormatsNavigationPane
            FormatsList {
            }
            
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id:  genresTab
        title: "Genres"
        description : "Song By Genre"
        NavigationPane {
            id: songGenresNavigationPane
            GenresList {
            }
            
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id:  topDownloadsTab
        title: "Downloads"
        description : "Top Downloaded Songs"
        NavigationPane {
            id: songTopDownloadedNavigationPane
            SongsTopDownloaded {
                navigationPane: songTopDownloadedNavigationPane
            }
            
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id:  topFavouritedTab
        title: "Favourited"
        description : "Top Favourited Songs"
        NavigationPane {
            id: songTopFavouritedNavigationPane
            SongsTopFavourited {
                navigationPane: songTopFavouritedNavigationPane
            }
            
            onPopTransitionEnded: {
                page.destroy()
            }
        }
    }
    Tab {
        id:  topScoredTab
        title: "Scored"
        description : "Top Scored Songs"
        NavigationPane {
            id: songTopScoredNavigationPane
            SongsTopScored {
                navigationPane: songTopScoredNavigationPane
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