import bb.cascades 1.1

TabbedPane {
    id: rootTabbedPane

    Tab {
        title: "Song Formats"
        FormatsList {
        }
    }
    Tab {
        title: "Genres"
        GenresList {
        }
    }
    
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            id: settingsActionItem
            title: "Settings";
            onTriggered : {
                //Invoke.invokeExecute("","","invokeSettings");  // tab/page = "" : Current tab
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
}