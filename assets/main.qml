import bb.cascades 1.1

TabbedPane {
    id: rootTabbedPane

    Tab {
        title: "Formats"
        description : "Song Formats"
        NavigationPane {
            id: songFormatsNavigationPane
            FormatsList {
            }
            
            onPopTransitionEnded: {
                console.debug(page.objectName)
                page.destroy()
            }
        }
    }
    Tab {
        title: "Genres"
        description : "Song Genres"
        NavigationPane {
            id: songGenresNavigationPane
            GenresList {
            }
            
            onPopTransitionEnded: {
                console.debug(page.objectName)
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