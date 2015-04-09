import bb.cascades 1.3
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
                    text: qsTr("Order") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.currentOrder : -1
                    colorScheme: Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright ? 0 : 1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Pattern") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.currentPattern : -1
                    colorScheme: Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright ? 0 : 1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Row") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.currentRow : -1
                    colorScheme: Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright ? 0 : 1
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
                    number: song ? song.playingChannels : -1
                    colorScheme: Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright ? 0 : 1
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
                    text: qsTr("Speed") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.currentSpeed : -1
                    colorScheme: Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright ? 0 : 1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Tempo") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.currentTempo : -1
                    colorScheme: Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright ? 0 : 1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                BlackLabel {
                    text: qsTr("Volume") + Retranslate.onLanguageChanged
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Left
                }
                LCDDigits {
                    number: song ? song.masterVolume : -1
                    colorScheme: Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright ? 0 : 1
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    numDigits: 3
                }
            }
        }
    }
}