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

        background: back.imagePaint

        attachedObjects: [
            Timer {
                id: lightTimer
                interval: 100
                repeat: true
                running: true 
                onTriggered: {
                    block.angleX += block.angleStepX
                    block.angleY += block.angleStepY

                    block.scalePhase += block.scaleStep
                    block.rotatePhase += block.rotateStep

                    block.translationX = Math.sin(block.angleX * 0.01745329251) * 117
                    block.translationY = Math.cos(block.angleY * 0.01745329251) * 97

                    var a
                    a = (block.rotateMax - block.rotateMin)
                    block.rotationZ = (Math.sin(block.rotatePhase * 0.01745329251) * a/2 + block.rotateMin) 

                    a = (block.scaleMax - block.scaleMin)
                    var m = (Math.sin(block.scalePhase * 0.01745329251) + 1) * a/2 + block.scaleMin 

                    block.scaleX = m
                    block.scaleY = m
                }
            },
            ImagePaintDefinition {
                id:back
                repeatPattern: RepeatPattern.XY
                imageSource: "asset:///images/backgrounds/view_back.amd"
            }
        ]
        minHeight: 2000
        minWidth: 2000
        implicitLayoutAnimationsEnabled: false
    }
}