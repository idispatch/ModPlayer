import bb.cascades 1.0
import QtQuick 1.0

Container {
    id: indicatorView
    
    property real displayOpacity: 0.5
    property int showTimeout: 800
    property int fadeDelay: 500
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
        opacity = 0.0
        textChanged.connect(showIndicator)
    }

    function showIndicator() {
        indicatorTimer.stop()
        indicatorView.opacity = displayOpacity
        indicatorView.visible = true
        indicatorTimer.start()
    }
    function hideIndicator() {
        indicatorTimer.stop()
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
        FadeTransition {
            id: hideAnimation
            fromOpacity: indicatorView.displayOpacity
            toOpacity: 0.0
            delay: indicatorView.fadeDelay
            onEnded: {
                indicatorView.visible = false
            }
        }
    ]
    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundImage
            imageSource: "asset:///images/backgrounds/indicator.amd"
        },
        Timer {
            id: indicatorTimer
            interval: indicatorView.showTimeout
            repeat: false
            onTriggered: {
                indicatorView.hideIndicator()
            }
        }
    ]
}