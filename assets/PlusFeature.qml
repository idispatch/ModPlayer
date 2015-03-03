import bb.cascades 1.3
import player 1.0

Container {
    horizontalAlignment: HorizontalAlignment.Center
    layout: DockLayout {}
    property alias extendedVersion: placeholder.controls
    Container {
        id: placeholder
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        opacity: app.isExtendedVersion ? 1.0 : 0.4
    }
    Container {
        id: buyPlaceholder
        layout: DockLayout {}
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        visible: !app.isExtendedVersion
        gestureHandlers: [
            TapHandler {
                onTapped: {
                    app.pleaseBuy()
                }
            }
        ]
        LockIcon {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            scaleX: 0.8
            scaleY: 0.8
            onCreationCompleted: {
                if(!app.isExtendedVersion) {
                    wiggle.play()
                }
            }
            animations: [
                SequentialAnimation {
                    id: wiggle
                    repeatCount: AnimationRepeatCount.Forever
                    property real totalTime: 300
                    property real angle: 30
                    property variant style: StockCurve.BackIn
                    RotateTransition {
                        fromAngleZ: wiggle.angle / 2
                        toAngleZ: -wiggle.angle / 2
                        duration: wiggle.totalTime / 2
                        easingCurve: wiggle.style
                    }
                    RotateTransition {
                        fromAngleZ: -wiggle.angle / 2
                        toAngleZ: wiggle.angle / 2
                        duration: wiggle.totalTime / 2
                        easingCurve: wiggle.style
                    }
                }
            ]
        }
    }
}