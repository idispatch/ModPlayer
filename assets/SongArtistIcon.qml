import bb.cascades 1.0
import player 1.0

WebImageView {
    property variant song
    visible: song && song.artistId > 0 && !song.isLocal && song.isTrackerSong
    url: song ? "http://api.modarchive.org/image-member.php?id=%1".arg(song.artistId) : ""
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