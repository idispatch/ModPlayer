import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Page {
    titleBar: PlayerTitleBar {
        title: {
            var currentSong = app.player.currentSong
            if(currentSong.songLoaded) {
                var fileName = Global.fileNameOnly(currentSong.fileName)
                return qsTr("Pattern %1 (%2)").arg(currentSong.currentPattern).arg(fileName)
            } else {
                return qsTr("Patterns")
            }
        }
    }
    ViewContainer {
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            TGroupContainer {
                PatternView {
                    song: app.player.currentSong
                    visible: app.player.currentSong.songLoaded
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
        }
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
        },
        RemoveFavouriteActionItem {
            currentSong: app.player.currentSong.songLoaded
        },
        AppWorldActionItem{}
    ]
}
