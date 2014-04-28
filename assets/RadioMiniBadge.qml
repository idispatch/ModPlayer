import bb.cascades 1.0

ImageView {
    property variant song
    visible: song && song.isHttpSong
    imageSource: {
        if(song) {
            if(song.fileName.indexOf("rockradio") != -1) {
                return "asset:///images/formats/icon-rockradio.png"
            } else if(song.fileName.indexOf("jazzradio") != -1) {
                return "asset:///images/formats/icon-jazzradio.png"
            } else if(song.fileName.indexOf("sky.fm") != -1) {
                return "asset:///images/formats/icon-skyfm.png"
            }
        }
        return "asset:///images/formats/icon-difm.png"
    }
    horizontalAlignment: HorizontalAlignment.Left
    verticalAlignment: VerticalAlignment.Center
    scalingMethod: ScalingMethod.Fill
    loadEffect: ImageViewLoadEffect.FadeZoom
    preferredWidth: 128
    preferredHeight: 128
    minWidth: 128
    maxWidth: 128
    minHeight: 128
    maxHeight: 128
}