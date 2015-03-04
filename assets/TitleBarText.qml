import bb.cascades 1.3
import player 1.0

HorizontalContainer {
    property string title
    leftPadding: 10
    Label {
        text: title
        textStyle {
            color: Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright ? Color.Black : Color.White
            fontSize: FontSize.Large
        }
        verticalAlignment: VerticalAlignment.Center
    }
}