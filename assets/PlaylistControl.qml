import bb.cascades 1.0
import QtQuick 1.0
import player 1.0

Container {
    id: playlistCounter
    layout: DockLayout {}
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    touchPropagationMode: TouchPropagationMode.Full
    overlapTouchPolicy: OverlapTouchPolicy.Allow
    leftPadding: 20
    rightPadding: 20
    bottomPadding: 20
    function updateDisplay() {
        if(app.player.playlist.count > 0) {
            hideAnimation.stop()
            showAnimation.play()
            displayTimer.restart()
        } else {
            displayTimer.stop()
            showAnimation.stop()
            hideAnimation.play()
        }
    }
    onCreationCompleted: {
        app.player.playlist.countChanged.connect(updateDisplay)
        app.player.playlist.currentChanged.connect(updateDisplay)
    }
    attachedObjects: [
        Timer {
            id: displayTimer
            interval: 4000
            repeat: false
            onTriggered: { 
                showAnimation.stop()
                hideAnimation.play() 
            }
        }
    ]
    Container {
        horizontalAlignment: HorizontalAlignment.Right
        verticalAlignment: VerticalAlignment.Bottom
        scaleX: 0.0
        scaleY: 0.0
        animations: [
            ScaleTransition {
                id: showAnimation
                duration: 300
                toX: 1.0
                toY: 1.0
                easingCurve: StockCurve.SineIn
                onStarted: {
                    playlistCounter.visible = true
                }
            },
            ScaleTransition {
                id: hideAnimation
                duration: 300
                toX: 0.0
                toY: 0.0
                easingCurve: StockCurve.SineOut
                onEnded: {
                    playlistCounter.visible = false
                }
            }
        ]
        HorizontalContainer {
            leftPadding: 14
            rightPadding: 14
            topPadding: 6
            bottomPadding: 6
            background: textBackground.imagePaint
            attachedObjects: [
                ImagePaintDefinition {
                    id: textBackground
                    repeatPattern: RepeatPattern.Fill
                    imageSource: "asset:///images/backgrounds/playlist_counter.amd"
                }
            ]
            BlackLabel {
                text: qsTr("%1 of %2").arg(app.player.playlist.position + 1).arg(app.player.playlist.count) + Retranslate.onLanguageChanged
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    textAlign: TextAlign.Center
                    fontStyle: FontStyle.Italic
                    fontSize: FontSize.XSmall
                }
            }
        }
    }
}
