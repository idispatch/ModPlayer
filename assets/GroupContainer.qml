import bb.cascades 1.3
import player 1.0

Container {
    id: groupContainer
    property bool semiTransparent: false
    horizontalAlignment: HorizontalAlignment.Fill
    background: backgroundImagePaint.imagePaint
    topPadding: 12
    bottomPadding: 12
    leftPadding: 16
    rightPadding: 16
    topMargin: 16
    bottomMargin: 16
    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundImagePaint
            repeatPattern: RepeatPattern.Fill
            imageSource: groupContainer.semiTransparent ? "asset:///images/backgrounds/container_back_85.amd" : "asset:///images/backgrounds/container_back.amd"
        }
    ]
}