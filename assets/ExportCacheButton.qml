import bb.cascades 1.3
import bb.system 1.2
import player 1.0
import "functions.js" as Global

Container {
    horizontalAlignment: HorizontalAlignment.Center
    PlusFeature {
        extendedVersion: Button {
            text: qsTr("Export Cache") + Retranslate.onLanguageChanged
            horizontalAlignment: HorizontalAlignment.Center
            topMargin: 40
            bottomMargin: 40
            enabled: app.cache.currentFiles > 0 && app.isExtendedVersion
            onClicked: {
                confirmExportingSongCache.show()
            }
            attachedObjects: [
                SystemDialog {
                    id: confirmExportingSongCache
                    title: qsTr("Confirm") + Retranslate.onLanguageChanged
                    body: qsTr("The song cache will now be exported to the 'ModPlayer' directory in your device downloads directory") + Retranslate.onLanguageChanged
                    onFinished: {
                        if (result != SystemUiResult.ConfirmButtonSelection)
                            return;
                        var thisProgress = progress
                        app.cache.progressUpdate.connect(function(percent, fileName) {
                            thisProgress.progress = percent
                            thisProgress.body = fileName
                            thisProgress.show()
                        })
                        progress.show()
                        app.cache.exportCache("ModPlayer")
                        progress.cancel()
                        cacheExportToast.show()
                    }
                },
                ProgressToast {
                    id: progress
                },
                SystemToast {
                    id: cacheExportToast
                    body: qsTr("The song cache has been exported successfully to the 'ModPlayer' directory in your device downloads directory") + Retranslate.onLanguageChanged
                }
            ]
        }
    }
}