import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: instrumentsView
    property string mode: "samples"
    titleBar: PlayerTitleBar {
        title: {
            var currentSong = app.player.currentSong
            if(currentSong.songLoaded) {
                var fileName = Global.fileNameOnly(currentSong.fileName)
                if(mode == 'samples') {
                    return qsTr("Samples of %1 (%2)").arg(fileName).arg(currentSong.samples)
                } else {
                    return qsTr("Instruments of %1 (%2)").arg(fileName).arg(currentSong.instruments)
                }
            } else {
                return qsTr("Instruments and Samples")
            }
        }
    }
    ViewContainer {
        InstrumentsView {}
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
