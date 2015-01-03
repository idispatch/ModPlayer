import bb.cascades 1.0
import bb.system 1.0
import player 1.0
import "functions.js" as Global

Container {
    horizontalAlignment: HorizontalAlignment.Center
    PlusFeature {
        extendedVersion: Button {
            text: qsTr("Import My Songs") + Retranslate.onLanguageChanged
            horizontalAlignment: HorizontalAlignment.Center
            topMargin: 40
            bottomMargin: 40
            enabled: app.isExtendedVersion
            onClicked: {
                app.player.importSongs()
            }
        }
    }
}