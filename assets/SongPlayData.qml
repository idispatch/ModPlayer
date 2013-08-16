import bb.cascades 1.0
import player 1.0

HorizontalContainer {
    property variant song

    visible: song != null && song.songLoaded
    horizontalAlignment: HorizontalAlignment.Fill
    
    topMargin: 16
    bottomMargin: 16

    VerticalContainer {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        }

        background: backgroundImagePaint.imagePaint

        topPadding: 12
        bottomPadding: 12
        leftPadding: 16
        rightPadding: 16

        rightMargin: 40

        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Order"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song != null ? song.currentOrder : -1
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Pattern"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song != null ? song.currentPattern : -1
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Row"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song != null ? song.currentRow : -1
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Channels"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song != null ? song.playingChannels : -1
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
    }
    VerticalContainer {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        }

        background: backgroundImagePaint.imagePaint

        topPadding: 12
        bottomPadding: 12
        leftPadding: 16
        rightPadding: 16

        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Speed"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song != null ? song.currentSpeed : -1
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Tempo"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song != null ? song.currentTempo : -1
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Volume"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song != null ? song.masterVolume : -1
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
    }
    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundImagePaint
            repeatPattern: RepeatPattern.Fill
            imageSource: "asset:///images/backgrounds/container_back.amd"
        }
    ]
}