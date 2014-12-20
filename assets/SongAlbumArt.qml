import bb.cascades 1.0
import player 1.0

AlbumArtView {
    property variant song

    fileName: song ? song.fileName : ""
    visible: song && song.fileName.length > 0 && !song.isTrackerSong && !song.isHttpSong

    scalingMethod: ScalingMethod.Fill
    loadEffect: ImageViewLoadEffect.None

    horizontalAlignment: HorizontalAlignment.Center

    topMargin: 20
    bottomMargin: 20

    minHeight: 400
    maxHeight: 450

    minWidth: 400
    maxWidth: 450

    scaleX: 0.0
    scaleY: 0.0

    onAlbumArtLoading: {
        loadingAnimation.stop()
        unloadingAnimation.play()
    }

    onAlbumArtLoaded: {
        unloadingAnimation.stop()
        loadingAnimation.play()
    }

    animations: [
        ScaleTransition {
            id: loadingAnimation
            toX: 1.0
            toY: 1.0
            duration: 350
            easingCurve: StockCurve.BackIn
        },
        ScaleTransition {
            id: unloadingAnimation
            toX: 0.0
            toY: 0.0
            duration: 350
            easingCurve: StockCurve.BackOut
        }
    ]
}