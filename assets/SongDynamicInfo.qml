import bb.cascades 1.0
import player 1.0

HorizontalContainer {
    property variant song

    visible: song && song.songLoaded
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
                BlackLabel {
                    text: qsTr("Order")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.currentOrder : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Pattern")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.currentPattern : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Row")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.currentRow : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Channels")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.playingChannels : -1
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
                BlackLabel {
                    text: qsTr("Speed")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.currentSpeed : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Tempo")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.currentTempo : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Volume")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.masterVolume : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
        }
    }
}