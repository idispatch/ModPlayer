import bb.cascades 1.0

Container {
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
            imageSource: "asset:///images/backgrounds/container_back.amd"
        }
    ]
}