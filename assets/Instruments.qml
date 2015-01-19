import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Page {
    titleBar: PlayerTitleBar {
        title: {
            var currentSong = app.player.currentSong
            if(currentSong.songLoaded) {
                var fileName = Global.fileNameOnly(currentSong.fileName)
                if(instrumentsChildView.mode == 'samples') {
                    return qsTr("Samples of %1 (%2)").arg(fileName).arg(currentSong.samples) + Retranslate.onLanguageChanged
                } else {
                    return qsTr("Instruments of %1 (%2)").arg(fileName).arg(currentSong.instruments) + Retranslate.onLanguageChanged
                }
            } else {
                return qsTr("Instruments and Samples") + Retranslate.onLanguageChanged
            }
        }
    }
    Container {
        layout: DockLayout {}
        WallpaperView {}
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            InstrumentsView {
                id: instrumentsChildView
                leftPadding: 20
                rightPadding: 20
                topPadding: 20
                bottomPadding: 20
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
        instrumentsChildView.load()
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
