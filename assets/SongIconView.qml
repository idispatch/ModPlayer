import bb.cascades 1.0
import player 1.0

ImageView {
    property variant song

    verticalAlignment: VerticalAlignment.Center
    scalingMethod: ScalingMethod.Fill
    imageSource: song ? song.iconPath : ""
    loadEffect: ImageViewLoadEffect.FadeZoom
    preferredHeight: 128
    preferredWidth: 128
    minHeight: 128
    maxHeight: 128
    minWidth: 128 
    maxWidth: 128
}