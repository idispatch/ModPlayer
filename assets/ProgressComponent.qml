import bb.cascades 1.0

Container {
    layout: DockLayout {}
    property alias running: activityIndicator.running
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
        animations: [
            ScaleTransition {
                id: startAnimation
                fromX: 0.0
                fromY: 0.0
                toX: 1.0
                toY: 1.0
                duration: 500
                easingCurve: StockCurve.BackOut
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
