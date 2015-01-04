import bb.cascades 1.0
import player 1.0

Container {
    property string colorName: "Color"
    property string colorValue: "#ff00ff00"
    layout: DockLayout {}
    background: Color.Black
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    topPadding: 1
    bottomPadding: 1
    leftPadding: 1
    rightPadding: 1
    Container {
        layout: DockLayout {}
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        background: Color.create(colorValue)
        bottomPadding: 8
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            background: cellTextBackground.imagePaint
            leftPadding: 20
            rightPadding: 20
            topPadding: 5
            bottomPadding: 8
            Label {
                text: colorName
                textStyle {
                    base: SystemDefaults.TextStyles.SmallText
                    fontWeight: FontWeight.W100
                    fontStyle: FontStyle.Italic
                    color: Color.White
                    fontSize: FontSize.XXSmall
                }
            }
            attachedObjects: [
                ImagePaintDefinition {
                    id: cellTextBackground
                    repeatPattern: RepeatPattern.Fill
                    imageSource: "asset:///images/cover/cover_text_back.amd"
                }
            ]
        }
    }
}
