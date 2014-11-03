import bb.cascades 1.3

Container {
    layout: DockLayout {}
    property alias running: activityIndicator.running
    property real scaleFactor: 0.90
    property int effectDuration: 100
    visible: running
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    topPadding: 200
    GroupContainer {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        ActivityIndicator {
            id: activityIndicator
            running: false
            preferredHeight: 128
            preferredWidth: preferredHeight
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }
        onTouch: {
            if (event.touchType == TouchType.Down) {
                releaseAnimation.stop()
                touchAnimation.play()
            } else if(event.touchType == TouchType.Cancel || event.touchType == TouchType.Up) {
                touchAnimation.stop()
                releaseAnimation.play()
            }
        }
        animations: [
            ScaleTransition {
                id: startAnimation
                fromX: 0.0
                fromY: 0.0
                toX: 1.0
                toY: 1.0
                duration: 500
                easingCurve: StockCurve.BackOut
            },
            ScaleTransition {
                id: touchAnimation
                toX: scaleFactor
                toY: scaleFactor
                duration: effectDuration
                easingCurve: StockCurve.BackOut
            },
            ScaleTransition {
                id: releaseAnimation
                toX: 1.0
                toY: 1.0
                duration: effectDuration
                easingCurve: StockCurve.BounceOut
            }
        ]
    }
    function start() {
        running = true
        startAnimation.play()
    }
    function stop() {
        startAnimation.stop()
        running = false
    }
}
