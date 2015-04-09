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
            imageSource: {
                if(groupContainer.semiTransparent) {
                    if(Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright) {
                        return "asset:///images/backgrounds/container_bright_85.amd"
                    } else {
                        return "asset:///images/backgrounds/container_dark_85.amd"
                    }
                } else {
                    if(Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright) {
                        return "asset:///images/backgrounds/container_bright.amd"
                    } else {
                        return "asset:///images/backgrounds/container_dark.amd"
                    }
                }
            }
        }
    ]
}