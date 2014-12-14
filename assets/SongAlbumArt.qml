import bb.cascades 1.0
import player 1.0

AlbumArtView {
    property variant song

    fileName: song ? song.fileName : ""
    visible: song && song.fileName.length > 0 && !song.isTrackerSong && !song.isHttpSong

    scalingMethod: ScalingMethod.Fill
    loadEffect: ImageViewLoadEffect.FadeZoom

    horizontalAlignment: HorizontalAlignment.Center

    topMargin: 20
    bottomMargin: 20

    minHeight: 200
    maxHeight: 450

    minWidth: 200
    maxWidth: 450
}