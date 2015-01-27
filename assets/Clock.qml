import bb.cascades 1.0
import player 1.0

Container {
    id: clockDisplay
    signal willStartSetUp()
    signal didFinishSetUp()
    property int preferredSize: app.screenWidth > 1300 ? 700 : (Math.max(app.screenWidth, app.screenHeight) < 800 ? 300 : 500)

    property real tickRadius: preferredSize / 2 - preferredSize * 0.116
    property real tickCenterX: preferredSize / 2 - 21
    property real tickCenterY: preferredSize / 2 - 8
    property real tickStep: 6.0
    property real clockValue: 360
    property real clockOfferedValue
    property bool setupMode: false

    minWidth: preferredSize
    minHeight: preferredSize
    maxWidth: preferredSize
    maxHeight: preferredSize
    preferredHeight: preferredSize
    preferredWidth: preferredSize
    layout: AbsoluteLayout {}
    touchBehaviors: TouchBehavior {
        eventReactions: [
            TouchReaction {
                phase: PropagationPhase.AtTarget
                response: TouchResponse.StartTracking
                eventType: TouchType.Down
            },
            TouchReaction {
                phase: PropagationPhase.AtTarget
                response: TouchResponse.StartTracking
                eventType: TouchType.Move
            }
        ]
    }
    onTouchCapture: {
        if(setupMode) {
            if(event.isDown()) {
                willStartSetUp()
            } else if(event.isUp()) {
                didFinishSetUp()
            } else  if(event.isCancel()) {
                didFinishSetUp()
            } else if(event.isMove()) {
                return
            }
        }
    }
    onTouch: {
        if(setupMode) {
            if(event.isDown() || event.isMove()) {
                var dx = -preferredWidth / 2 + event.localX
                var dy = -preferredHeight / 2 + event.localY
                if(Math.sqrt(dx * dx + dy * dy) < (preferredWidth / 2) * 0.9) {
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
    }
    ImageView {
        imageSource: "asset:///images/clock/clock-face.png"
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        scalingMethod: ScalingMethod.Fill
        loadEffect: ImageViewLoadEffect.None
        preferredWidth: preferredSize
        preferredHeight: preferredSize
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        preferredHeight: clockDisplay.preferredSize
        preferredWidth: clockDisplay.preferredSize
        layout: DockLayout {}
        LCDDigits {
            id: digitDisplay
            number: app.player.sleepTimer.sleepRemaining
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            touchPropagationMode: TouchPropagationMode.Full
            displayScale: app.screenHeight < 1000 ? 2.0 : (app.screenWidth > 1400 ? 4.0 : 3.0)
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
                        easingCurve: StockCurve.QuadraticIn
                    }
                    FadeTransition {
                        fromOpacity: 1.0
                        toOpacity: 0.0
                        duration: blinkAnimation.totalTime / 2
                        easingCurve: StockCurve.QuadraticOut
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
                loadEffect: ImageViewLoadEffect.None
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
                loadEffect: ImageViewLoadEffect.None
                preferredWidth: preferredSize
                preferredHeight: preferredSize
            }
        }
    ]
}
