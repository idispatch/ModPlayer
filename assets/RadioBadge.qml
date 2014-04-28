import bb.cascades 1.0
import player 1.0

ImageView {
    property variant song
    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    visible: app.player.currentSong.songLoaded && app.player.currentSong.isHttpSong
    loadEffect: ImageViewLoadEffect.FadeZoom
    scalingMethod: ScalingMethod.Fill
    imageSource: "asset:///images/radio/difm.png"
}
