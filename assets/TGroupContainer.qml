import bb.cascades 1.0

GroupContainer {
    property real scaleFactor: 0.98
    property int effectDuration: 100
    /*
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
            id: touchAnimation
            toX: scaleFactor
            toY: scaleFactor
            duration: effectDuration
            easingCurve: StockCurve.QuadraticIn
        },
        ScaleTransition {
            id: releaseAnimation
            toX: 1.0
            toY: 1.0
            duration: effectDuration
            easingCurve: StockCurve.QuadraticOut
        }
    ]
    */
}
