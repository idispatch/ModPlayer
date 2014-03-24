import bb.cascades 1.0
import player 1.0

WebImageView {
    property variant song
    visible: song && song.artistId > 0
    url: song ? "http://modarchive.org/data/image-member.php?id=%1".arg(song.artistId) : ""
    verticalAlignment: VerticalAlignment.Center
    scalingMethod: ScalingMethod.Fill
    loadEffect: ImageViewLoadEffect.FadeZoom
    preferredHeight: 128
    preferredWidth: 128
    minHeight: 128
    maxHeight: 128
    minWidth: 128 
    maxWidth: 128
}