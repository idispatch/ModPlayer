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
        PlaylistControl {}
    }
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
        BuyActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
}
