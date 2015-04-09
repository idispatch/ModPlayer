import bb.cascades 1.3
import player 1.0

HorizontalContainer {
    property string title
    leftPadding: 10
    rightPadding: 10
    Label {
        id: titleLabel
        text: title
        textFit {
            mode: LabelTextFitMode.FitToBounds
            minFontSizeValue: 6.0
            maxFontSizeValue: 9.0
        }
        textStyle {
            color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright ? Color.Black : Color.White
            fontSize: titleLabel.text.length > 25 ? (titleLabel.text.length > 35 ? FontSize.Small : FontSize.Medium) : FontSize.Large
        }
        verticalAlignment: VerticalAlignment.Center
    }
}