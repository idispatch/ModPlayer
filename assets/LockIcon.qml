import bb.cascades 1.0
import player 1.0

ImageView {
    imageSource: "asset:///images/objects/lock.png"
    minHeight: 64
    maxHeight: 64
    minWidth: 64
    maxWidth: 64
    preferredWidth: 64
    preferredHeight: 64
    verticalAlignment: VerticalAlignment.Center
    visible: !app.isExtendedVersion
}