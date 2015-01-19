import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Page {
    titleBar: PlayerTitleBar {
        title: {
            var currentSong = app.player.currentSong
            if(currentSong.songLoaded) {
                var fileName = Global.fileNameOnly(currentSong.fileName)
                return qsTr("Pattern %1 (%2)").arg(currentSong.currentPattern).arg(fileName) + Retranslate.onLanguageChanged
            } else {
                return qsTr("Patterns") + Retranslate.onLanguageChanged
            }
        }
    }
    Container {
        layout: DockLayout {}
        WallpaperView {}
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            ScrollView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                GroupContainer {
                    PatternView {
                        song: app.player.currentSong
                        visible: app.player.currentSong.songLoaded
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
            }
        }
        SleepTimerDisplay {}
        PlaylistControl {}
    }
    function addBuyButton() {
        if(!app.isExtendedVersion) {
            var buyActionItem = buyAppComponentDefinition.createObject()
            if(buyActionItem) {
                addAction(buyActionItem, ActionBarPlacement.InOverflow)
            }
        }
    }
    onCreationCompleted: {
        addBuyButton()
    }
    attachedObjects: [
        ComponentDefinition {
            id: buyAppComponentDefinition
            source: "BuyActionItem.qml"
        }
    ]
    actions: [ 
        PlayActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        AddFavouriteActionItem {
            currentSong: app.player.currentSong.songLoaded
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        RemoveFavouriteActionItem {
            currentSong: app.player.currentSong.songLoaded
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        SleepTimerActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        SettingsMenuAction {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
}
