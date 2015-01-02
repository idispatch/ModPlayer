import bb.cascades 1.0
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
    LockIcon {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        visible: !app.isExtendedVersion
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