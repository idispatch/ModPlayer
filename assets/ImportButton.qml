import bb.cascades 1.0
import bb.system 1.0
import player 1.0
import "functions.js" as Global

Container {
    horizontalAlignment: HorizontalAlignment.Center
    HorizontalContainer {
        LockIcon {
        }
        Button {
            text: qsTr("Import My Songs") + Retranslate.onLanguageChanged
            horizontalAlignment: HorizontalAlignment.Center
            topMargin: 40
            bottomMargin: 40
            onClicked: {
                confirmImport.show()
            }
            attachedObjects: [
                SystemDialog {
                    id: confirmImport
                    title: qsTr("Confirm") + Retranslate.onLanguageChanged
                    body: qsTr("Would you like to import local songs and playlists?") + Retranslate.onLanguageChanged
                    onFinished: {
                        if (result == SystemUiResult.ConfirmButtonSelection) {
                            app.player.importSongs()
                            personalSongCount.updateCount()
                        }
                    }
                }
            ]
        }
    }
}