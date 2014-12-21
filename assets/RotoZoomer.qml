import bb.cascades 1.0
import QtQuick 1.0

Container {
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill

    minHeight: 2000
    minWidth: 2000

    Container {
        id: block

        property double scalePhase: 0
        property double rotatePhase: 63
        property double angleX: 0.0
        property double angleY: 0.0

        property double scaleStep: 3.1
        property double rotateStep: 0.35
        property double angleStepX: 3.7
        property double angleStepY: 2.5

        property double rotateMin: -180.0
        property double rotateMax: 360

        property double scaleMax: 3.2
        property double scaleMin: 1.5

        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        minHeight: 2000
        minWidth: 2000
        
        implicitLayoutAnimationsEnabled: false
        
        background: back.imagePaint

        translationX: Math.sin(angleX * 0.01745329251) * 117
        translationY: Math.cos(angleY * 0.01745329251) * 97

        rotationZ: (Math.sin(rotatePhase * 0.01745329251) * (rotateMax - rotateMin)/2 + rotateMin) 

        scaleX: (Math.sin(scalePhase * 0.01745329251) + 1) * (scaleMax - scaleMin)/2 + scaleMin
        scaleY: (Math.sin(scalePhase * 0.01745329251) + 1) * (scaleMax - scaleMin)/2 + scaleMin
        
        attachedObjects: [
            Timer {
                id: rotozoomTimer
                interval: 100
                repeat: true
                running: true 
                onTriggered: {
                    block.angleX += block.angleStepX
                    block.angleY += block.angleStepY
                    block.scalePhase += block.scaleStep
                    block.rotatePhase += block.rotateStep
                }
            },
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.XY
                imageSource: "asset:///images/backgrounds/view_back.amd"
            }
        ]
    
        onCreationCompleted: {
            Application.fullscreen.connect(function(){
                rotozoomTimer.running = true
            })
            Application.invisible.connect(function(){
                rotozoomTimer.running = false
            })
            Application.asleep.connect(function(){
                rotozoomTimer.running = false
            })
            Application.thumbnail.connect(function(){
                rotozoomTimer.running = false
            })
        }
    }
}