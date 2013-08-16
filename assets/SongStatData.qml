import bb.cascades 1.0
import player 1.0

HorizontalContainer {
    property variant song

    visible: song != null
    horizontalAlignment: HorizontalAlignment.Fill

    VerticalContainer {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        }
        rightMargin: 50
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
        leftMargin: 50
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
}