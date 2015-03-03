import bb.cascades 1.3
import player 1.0

Container {
    id: hint
    layout: DockLayout {}
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    background: Color.create("#A0000000")
    topPadding: 50
    leftPadding: 20
    rightPadding: 20
    visible: app.isFirstLaunch
    function startFirstTime() {
        app.isFirstLaunch = false
        app.player.importSongs()
    }
    GroupContainer {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Top
        animations: [
            TranslateTransition {
                id: hideAnimation
                toX: 1500
                duration: 500
                easingCurve: StockCurve.CubicOut
                onStateChanged: {
                    if(state == AnimationState.Ended) {
                        startFirstTime()
                    }
                }
            }
        ]
        onTouch: {
            hideAnimation.play()
        }
        HorizontalContainer {
            leftPadding: 10
            rightPadding: 10
            topPadding: 10
            ImageView {
                imageSource: "asset:///images/objects/pulldown.png"
                verticalAlignment: VerticalAlignment.Center
                minWidth: 128
                preferredHeight: 128
                preferredWidth: 128
            }
            BlackLabel {
                verticalAlignment: VerticalAlignment.Center
                leftMargin: 20
                text: qsTr("<b>Usage Hint</b><p>Swipe down from the top bezel to configure ModPlayer settings.</p>") + Retranslate.onLanguageChanged
                multiline: true
                textFormat: TextFormat.Html
                textStyle {
                    textAlign: TextAlign.Left
                    fontStyle: FontStyle.Italic
                    base: SystemDefaults.TextStyles.BodyText
                }
            }
        }
    }
}
