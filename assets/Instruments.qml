import bb.cascades 1.0
import "functions.js" as Global

Page {
    titleBar: PlayerTitleBar {
        title: {
            var currentSong = app.player.currentSong
            if(currentSong.songLoaded) {
                var fileName = Global.fileNameOnly(currentSong.fileName)
                if(instrumentsChildView.mode == 'samples') {
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
        InstrumentsView {
            id: instrumentsChildView
            leftPadding: 20
            rightPadding: 20
            topPadding: 20
            bottomPadding: 20
        }
    }
    onCreationCompleted: {
        instrumentsChildView.load()
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
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
}
