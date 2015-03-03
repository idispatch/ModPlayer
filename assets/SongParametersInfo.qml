import bb.cascades 1.3
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
                BlackLabel {
                    text: qsTr("Orders") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.orders : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Patterns") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.patterns : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Channels") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.channels : -1
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
                    text: qsTr("Instruments") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.instruments : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Samples") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.samples : -1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
        }
    }
}