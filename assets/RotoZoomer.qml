import bb.cascades 1.0
import QtQuick 1.0
import player 1.0

Container {
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    layout: AbsoluteLayout {}
    implicitLayoutAnimationsEnabled: false
    background: backgroundPaintOuter.imagePaint
    Container {
        id: block
        layoutProperties: AbsoluteLayoutProperties {
            positionX: -layoutHandler.layoutFrame.width/2
            positionY: -layoutHandler.layoutFrame.height/2
        }

        property double scalePhase: 0
        property double rotatePhase: 63
        property double angleX: 0.0
        property double angleY: 0.0

        property double scaleStep: 0.25
        property double rotateStep: 0.10
        property double angleStepX: 2.2
        property double angleStepY: 1.1

        property double rotateMin: -180.0
        property double rotateMax: 360

        property double scaleMax: 2.5
        property double scaleMin: 1.0

        property bool animationEnabled: app.player.playback.configuration.animationEnabled

        preferredWidth: layoutHandler.layoutFrame.width * 2
        preferredHeight: layoutHandler.layoutFrame.height * 2

        background: backgroundPaint.imagePaint
        implicitLayoutAnimationsEnabled: false

        translationX: animationEnabled ? Math.sin(angleX * 0.01745329251) * 117 : 0
        translationY: animationEnabled ? Math.cos(angleY * 0.01745329251) * 97 : 0

        rotationZ: animationEnabled ? (Math.sin(rotatePhase * 0.01745329251) * (rotateMax - rotateMin)/2 + rotateMin) : 0 

        scaleX: animationEnabled ? (Math.sin(scalePhase * 0.01745329251) + 1) * (scaleMax - scaleMin)/2 + scaleMin : 1.0
        scaleY: animationEnabled ? (Math.sin(scalePhase * 0.01745329251) + 1) * (scaleMax - scaleMin)/2 + scaleMin : 1.0

        attachedObjects: [
            Timer {
                id: rotozoomTimer
                interval: 50
                repeat: true
                running: true
                onTriggered: {
                    if(block.animationEnabled) {
                        block.angleX += block.angleStepX
                        block.angleY += block.angleStepY
                        block.scalePhase += block.scaleStep
                        block.rotatePhase += block.rotateStep
                    }
                }
            },
            ImplicitAnimationController {
                enabled: false
            },
            ImagePaintDefinition {
                id: backgroundPaint
                repeatPattern: RepeatPattern.XY
                imageSource: "asset:///images/backgrounds/view_back.amd"
            }
        ]
        onCreationCompleted: {
            Application.fullscreen.connect(function(){
                rotozoomTimer.running = app.player.playback.configuration.animationEnabled
            })
            var disableAnimationTimer = function(){
                rotozoomTimer.running = false
            }
            Application.invisible.connect(disableAnimationTimer)
            //Application.asleep.connect(disableAnimationTimer)
            Application.thumbnail.connect(disableAnimationTimer)
        }
    }
    attachedObjects:[
        LayoutUpdateHandler {
            id: layoutHandler
        },
        ImagePaintDefinition {
            id: backgroundPaintOuter
            repeatPattern: RepeatPattern.XY
            imageSource: "asset:///images/backgrounds/view_back.amd"
        }
    ]
}