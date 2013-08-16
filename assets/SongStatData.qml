import bb.cascades 1.0
import player 1.0

HorizontalContainer {
    property variant song

    visible: song != null
    horizontalAlignment: HorizontalAlignment.Fill

    topMargin: 16
    bottomMargin: 16

    VerticalContainer {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        }

        background: backgroundImagePaint.imagePaint
        rightMargin: 40

        topPadding: 12
        bottomPadding: 12
        leftPadding: 16
        rightPadding: 16

        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Orders"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song!=null ? song.orders : -1
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Patterns"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song!=null ? song.patterns : -1
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
                number: song!=null ? song.channels : -1
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
                text: "Instruments"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song!=null ? song.instruments : -1
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
        Container {
            layout: DockLayout {}
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Samples"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                number: song!=null ? song.samples : -1
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