import bb.cascades 1.0
import QtQuick 1.0
import player 1.0

ImageView {
    property bool mouseActive: false
    imageSource: "asset:///images/objects/mouse.png"
    horizontalAlignment: HorizontalAlignment.Right
    verticalAlignment: VerticalAlignment.Bottom
    translationY: 450
    gestureHandlers: [
        TapHandler {
            onTapped: hideMouseAnimation.play()
        }
    ]
    function showMouse() {
        if(!mouseActive) {
            mouseActive = true
            showMouseAnimation.play()
            mouseTimer.start()
        }
    }
    function hideMouse() {
        showMouseAnimation.stop()
        mouseTimer.stop()
        hideMouseAnimation.play()
        mouseActive = false
    }
    attachedObjects: [
        Timer {
            id: mouseTimer
            interval: 4000
            repeat: false
            onTriggered: {
                hideMouse()
            }
        }
    ]
    animations: [
        TranslateTransition {
            id: showMouseAnimation
            fromY: 450
            toY: 0
            duration: 1000
            easingCurve: StockCurve.BackIn
        },
        TranslateTransition {
            id: hideMouseAnimation
            toY: 450
            duration: 600
            easingCurve: StockCurve.BackOut
        }
    ]
}