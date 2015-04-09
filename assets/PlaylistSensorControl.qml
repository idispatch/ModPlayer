import bb.cascades 1.3
import player 1.0

GroupContainer {
    topPadding: 20
    bottomPadding: 40
    leftPadding: 20
    rightPadding: 20
    semiTransparent: true
    BlackLabel {
        text: qsTr("Playlist Control") + Retranslate.onLanguageChanged
        horizontalAlignment: HorizontalAlignment.Center
        bottomMargin: 40
        textStyle {
            base: SystemDefaults.TextStyles.TitleText
            fontWeight: FontWeight.Bold
        }
    }
    PlusFeature {
        extendedVersion: VerticalContainer {
            MultilineCheckBox {
                text: qsTr("On Device Shake Play Next") + Retranslate.onLanguageChanged
                checked: app.shakeSensorEnabled
                onCheckedChanged: {
                    app.shakeSensorEnabled = checked
                }
            }
            MultilineCheckBox {
                text: qsTr("On Proximity Sensor Play Next") + Retranslate.onLanguageChanged
                checked: app.proximitySensorEnabled
                onCheckedChanged: {
                    app.proximitySensorEnabled = checked
                }
            }
        }
    }
}