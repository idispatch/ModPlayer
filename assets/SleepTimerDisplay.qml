import bb.cascades 1.0
import QtQuick 1.0
import player 1.0

Container {
    id: sleepTimerDisplay
    layout: DockLayout {}
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    touchPropagationMode: TouchPropagationMode.Full
    overlapTouchPolicy: OverlapTouchPolicy.Allow
    leftPadding: 20
    rightPadding: 20
    bottomPadding: 20
    visible: app.player.sleepTimer.timerActive
    Container {
        horizontalAlignment: HorizontalAlignment.Left
        verticalAlignment: VerticalAlignment.Bottom
        HorizontalContainer {
            leftPadding: 20
            rightPadding: 20
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
                text: app.player.sleepTimer.status
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    color: Color.White
                    textAlign: TextAlign.Center
                    fontSize: FontSize.XXLarge
                    fontWeight: FontWeight.W500
                }
            }
        }
    }
}
