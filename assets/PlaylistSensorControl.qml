import bb.cascades 1.0
import player 1.0

GroupContainer {
    topPadding: 20
    bottomPadding: 40
    leftPadding: 20
    rightPadding: 20
    semiTransparent: true
    Label {
        text: qsTr("Playlist Control") + Retranslate.onLanguageChanged
        horizontalAlignment: HorizontalAlignment.Center
        bottomMargin: 40
        textStyle {
            base: SystemDefaults.TextStyles.TitleText
            fontWeight: FontWeight.Bold
            color: Color.Black
        }
    }
    PlusFeature {
        extendedVersion: VerticalContainer {
            HorizontalContainer {
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 40
                BlackLabel {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    text: qsTr("On Device Shake Play Next") + Retranslate.onLanguageChanged
                    multiline: true
                }
                CheckBox {
                    checked: app.shakeSensorEnabled
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                    onCheckedChanged: {
                        app.shakeSensorEnabled = checked
                    }
                }
            }
            HorizontalContainer {
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 40
                BlackLabel {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    text: qsTr("On Proximity Sensor Play Next") + Retranslate.onLanguageChanged
                    multiline: true
                }
                CheckBox {
                    checked: app.proximitySensorEnabled
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                    bottomMargin: 40
                    onCheckedChanged: {
                        app.proximitySensorEnabled = checked
                    }
                }
            }
        }
    }
}