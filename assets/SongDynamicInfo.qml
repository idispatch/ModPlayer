import bb.cascades 1.0
import player 1.0

HorizontalContainer {
    property variant song

    visible: song != null && song.songLoaded
    horizontalAlignment: HorizontalAlignment.Fill
    
    topMargin: 16
    bottomMargin: 16

    GroupContainer {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 10
        }
        rightMargin:20
        VerticalContainer {
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: "Order"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                    textStyle.color: Color.Black
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
                    textStyle.color: Color.Black
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
                    textStyle.color: Color.Black
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
                    textStyle.color: Color.Black
                }
                LCDDigits {
                    number: song != null ? song.playingChannels : -1
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
                    text: "Speed"
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                    textStyle.color: Color.Black
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
                    textStyle.color: Color.Black
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
                    textStyle.color: Color.Black
                }
                LCDDigits {
                    number: song != null ? song.masterVolume : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
        }
    }
}