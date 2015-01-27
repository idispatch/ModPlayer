import bb.cascades 1.0
import player 1.0

Container {
    id: clockDisplay
    property real tickRadius: 195
    property real tickCenterX: 230
    property real tickCenterY: 242
    property real tickStep: 5.0
    property real clockTime: 90.0
    property bool setupMode: false
    
    minWidth: 500
    minHeight: 500
    maxWidth: 500
    maxHeight: 500
    preferredHeight: 500
    preferredWidth: 500
    
    layout: AbsoluteLayout {}
    
    ImageView {
        imageSource: "asset:///images/clock/clock-face.png"
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        layout: DockLayout {}
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
        }
    }
    onSetupModeChanged: {
        if(setupMode) {
            setupTimer.start()
        } else {
            setupTimer.stop()
            digitDisplay.visible = true
        }
    }
    onCreationCompleted: {
        for(var angle = 0; angle < 360; angle += tickStep) {
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
                property real tickAngle
                property bool tickIsLight: (tickAngle + 90 - 0.5) % 360 < clockTime
                imageSource: tickIsLight ? "asset:///images/clock/tick-light.png" : "asset:///images/clock/tick-dark.png" 
                rotationZ: -180 + tickAngle
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: clockDisplay.tickCenterX + Math.cos(tickAngle * 0.01745329251994) * clockDisplay.tickRadius
                    positionY: clockDisplay.tickCenterY + Math.sin(tickAngle * 0.01745329251994) * clockDisplay.tickRadius
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
        },
        QTimer {
            id: setupTimer
            interval: 400
            singleShot: false
            onTimeout : {
                digitDisplay.visible = !digitDisplay.visible 
            }
        }
    ]
}