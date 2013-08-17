import bb.cascades 1.0
import player 1.0

HorizontalContainer {
    property variant song

    visible: song != null
    horizontalAlignment: HorizontalAlignment.Fill

    topMargin: 16
    bottomMargin: 16

    GroupContainer {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 10
        }
        horizontalAlignment: HorizontalAlignment.Fill
        rightMargin: 20
        VerticalContainer {
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: "Orders"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                    textStyle.color: Color.Black
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
                    textStyle.color: Color.Black
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
                    textStyle.color: Color.Black
                }
                LCDDigits {
                    number: song!=null ? song.channels : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
        }
    }
    GroupContainer {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 10
        }
        VerticalContainer {
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: "Instruments"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                    textStyle.color: Color.Black
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
                    textStyle.color: Color.Black
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
}