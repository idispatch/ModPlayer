import bb.cascades 1.3
import player 1.0

HorizontalContainer {
    property string title
    leftPadding: 10
    Label {
        text: title
        textStyle {
            color: app.player.lightTheme ? Color.Black : Color.White
            fontSize: FontSize.Large
        }
        verticalAlignment: VerticalAlignment.Center
    }
}