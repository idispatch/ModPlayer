import bb.cascades 1.0
import player 1.0

ImageView {
    property variant song
    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    visible: song.isHttpSong
    loadEffect: ImageViewLoadEffect.FadeZoom
    scalingMethod: ScalingMethod.Fill
    imageSource: {
        if(song) {
            if(song.fileName.indexOf("rockradio") != -1) {
                return "asset:///images/radio/rockradio.png"
            } else if(song.fileName.indexOf("jazzradio") != -1) {
                return "asset:///images/radio/jazzradio.png"
            } else if(song.fileName.indexOf("sky.fm") != -1) {
                return "asset:///images/radio/skyfm.png"
            }
        }
        return "asset:///images/radio/difm.png"
    }
}
