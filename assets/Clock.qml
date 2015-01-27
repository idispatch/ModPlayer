import bb.cascades 1.0
import player 1.0

Container {
    id: clockDisplay

    property real tickRadius: 195
    property real tickCenterX: 230
    property real tickCenterY: 242
    property real tickStep: 6.0
    property real clockValue
    property real clockOfferedValue
    property bool setupMode: false

    minWidth: 500
    minHeight: 500
    maxWidth: 500
    maxHeight: 500
    preferredHeight: 500
    preferredWidth: 500

    layout: AbsoluteLayout {
    }
    
    gestureHandlers: [
        TapHandler {
            onTapped: {
                if(setupMode) {
                    var dx = -preferredWidth / 2 + event.x
                    var dy = -preferredHeight / 2 + event.y
                    var radians = Math.atan2(dy, dx)
                    var degrees = radians * 180 / Math.PI
                    if(degrees < 0) {
                        degrees = Math.abs(degrees)
                    } else {
                        degrees = 360 - degrees
                    }
                    degrees = 360 - degrees + 90
                    if(degrees > 360) {
                        degrees = degrees - 360
                    }
                    clockDisplay.clockOfferedValue = degrees
                }
            }
        }
    ]

    ImageView {
        imageSource: "asset:///images/clock/clock-face.png"
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        layout: DockLayout {
        }
        preferredHeight: 500
        preferredWidth: 500
        LCDDigits {
            id: digitDisplay
            number: app.player.sleepTimer.sleepRemaining
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            displayScale: 3.0
            numDigits: 2
            colorScheme: 1
            animations: [
                SequentialAnimation {
                    id: blinkAnimation
                    repeatCount: AnimationRepeatCount.Forever
                    property real totalTime: 500
                    FadeTransition {
                        fromOpacity: 0.0
                        toOpacity: 1.0
                        duration: blinkAnimation.totalTime / 2
                        easingCurve: StockCurve.SineOut
                    }
                    FadeTransition {
                        fromOpacity: 1.0
                        toOpacity: 0.0
                        duration: blinkAnimation.totalTime / 2
                        easingCurve: StockCurve.SineOut
                    }
                }
            ]
        }
    }
    onSetupModeChanged: {
        if (setupMode) {
            blinkAnimation.play()
        } else {
            blinkAnimation.stop()
            digitDisplay.opacity = 1.0
        }
    }
    onCreationCompleted: {
        for (var angle = 0; angle < 360; angle += tickStep) {
            var tick = clockTick.createObject()
            tick.tickAngle = angle
            add(tick)
        }
        add(clockGloss.createObject())
    }
    attachedObjects: [
        ComponentDefinition {
            id: clockTick
            ImageView {
                property int tickAngle
                rotationZ: -180 + tickAngle
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: clockDisplay.tickCenterX + Math.cos(tickAngle * 0.01745329251994) * clockDisplay.tickRadius
                    positionY: clockDisplay.tickCenterY + Math.sin(tickAngle * 0.01745329251994) * clockDisplay.tickRadius
                }
                function updateDisplay() {
                    var tickIsLight = ((tickAngle + 90 - 0.5) % 360) < clockDisplay.clockValue
                    if(tickIsLight) {
                        imageSource = "asset:///images/clock/tick-light.png"
                    } else {
                        imageSource = "asset:///images/clock/tick-dark.png"
                    }
                }
                onTickAngleChanged: {
                    updateDisplay()
                }
                onCreationCompleted: {
                    updateDisplay()
                    clockDisplay.clockValueChanged.connect(updateDisplay)
                }
            }
        },
        ComponentDefinition {
            id: clockGloss
            ImageView {
                imageSource: "asset:///images/clock/clock-gloss.png"
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }
        }
    ]
}