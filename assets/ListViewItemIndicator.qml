import bb.cascades 1.0

Container {
    id: indicatorView

    property real displayOpacity: 1.0
    property int showTimeout:     800
    property int fadeDelay:       400
    property int fadeDuration:    400
    property string text: ""

    minHeight: 150
    minWidth: 150
    preferredHeight: 150
    preferredWidth: 150
    maxWidth: 150
    maxHeight: 150

    background: backgroundImage.imagePaint
    horizontalAlignment: HorizontalAlignment.Center
    verticalAlignment: VerticalAlignment.Center

    layout: DockLayout {}

    onCreationCompleted: {
        visible = false
        opacity = 1.0
        scaleX = 1.0
        scaleY = 1.0
        var thisObject = indicatorView
        textChanged.connect(function() {
            thisObject.showIndicator()
        })
    }
    function showIndicator() {
        hideAnimation.stop()
        indicatorView.opacity = displayOpacity
        indicatorView.scaleX = 1.0
        indicatorView.scaleY = 1.0
        indicatorView.visible = true
        hideAnimation.play()
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        Label {
            id: indicatorLabel
            text: indicatorView.text
            textStyle.fontSize: FontSize.XXLarge
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }
    }
    animations: [
        ParallelAnimation {
            id: hideAnimation
            delay: indicatorView.fadeDelay
            onEnded: {
                indicatorView.visible = false
            }
            FadeTransition {
                fromOpacity: indicatorView.displayOpacity
                toOpacity: 0.0
                duration: indicatorView.fadeDuration
                easingCurve: StockCurve.Linear
            }
            ScaleTransition {
                duration: indicatorView.fadeDuration
                fromX: 1.0
                toX: 3.0
                fromY: 1.0
                toY: 3.0
                easingCurve: StockCurve.CircularOut
            }
        }
    ]
    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundImage
            imageSource: "asset:///images/backgrounds/indicator.amd"
        }
    ]
}