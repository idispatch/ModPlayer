import bb.cascades 1.3

Container {
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    background: viewBackground.imagePaint
    attachedObjects: [
        ImagePaintDefinition {
            id: viewBackground
            repeatPattern: RepeatPattern.XY
            imageSource: "asset:///images/backgrounds/view_back.amd"
        }
    ]
}