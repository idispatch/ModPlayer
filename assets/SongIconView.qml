import bb.cascades 1.0
import player 1.0

ImageView {
    property variant song
    
    verticalAlignment: VerticalAlignment.Center
    scalingMethod: ScalingMethod.Fill
    imageSource: app.getIconPath(song)
    loadEffect: ImageViewLoadEffect.FadeZoom
    preferredHeight: 128
    preferredWidth: 128
    minHeight: preferredHeight
    maxHeight: preferredHeight
    minWidth: preferredWidth 
    maxWidth: preferredWidth
}